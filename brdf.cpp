// STL
#include <iostream>
#include <cmath>
#include <limits>

// ALTA headers
#include <core/common.h>
#include <core/ptr.h>
#include <core/plugins_manager.h>
#include <core/function.h>

// MTS headers
#include <mitsuba/mitsuba.h>
#include <mitsuba/render/bsdf.h>
#include <mitsuba/hw/basicshader.h>
#include <mitsuba/core/warp.h>
#include <mitsuba/core/vmf.h>
#include <mitsuba/core/transform.h>


MTS_NAMESPACE_BEGIN

//#define USE_VMF

class AltaBRDF : public BSDF {
public:
	/* Constructors & destructors */

	AltaBRDF(const Properties& props) : BSDF(props), _data(NULL), _func(NULL) {
		if(props.hasProperty("data-plugin")) {
			_data = plugins_manager::get_data(props.getString("data-plugin"));
			_data->load(props.getString("filename"));
		} else {
			_func = plugins_manager::load_function(props.getString("filename"));
		}
#ifdef USE_VMF
		_distrib.setKappa(100);
#endif
	}

	AltaBRDF(Stream *stream, InstanceManager *manager) : BSDF(stream, manager), _data(NULL), _func(NULL) {
		configure();
	}

	~AltaBRDF() {
	}

	/* Mitsuba interface */

	virtual void configure() {	
		
		m_components.clear();
		m_components.push_back(EDiffuseReflection | EFrontSide);
#ifdef USE_VMF
		m_components.push_back(EGlossyReflection| EFrontSide);
#endif
		m_usesRayDifferentials = false;
		
		BSDF::configure();
	}

	Spectrum getDiffuseReflectance(const Intersection &its) const {
		return Spectrum(0.0f);
	}
	
	Spectrum getReflectance(const Vector& wi, const Vector& wo) const {
		if (Frame::cosTheta(wi) <= 0 || Frame::cosTheta(wo) <= 0) {
			return Spectrum(0.0f);
		}

		double cart[6];
		cart[0] = wi[0];
		cart[1] = wi[1];
		cart[2] = wi[2];
		cart[3] = wo[0];
		cart[4] = wo[1];
		cart[5] = wo[2];

			
		/* Return the value of the BRDF from the function object */
		if(!_data) {
			vec x(_func->dimX());
			params::convert(&cart[0], params::CARTESIAN, _func->input_parametrization(), &x[0]);
			vec y = _func->value(x);
			Spectrum res;
			if(_func->dimY() == 3) {
				res.fromLinearRGB(std::max(y[0], 0.0), std::max(y[1], 0.0), std::max(y[2], 0.0));
			} else {
				res = Spectrum(std::max(y[0], 0.0));
			}
			return res;

		/* Treat the case of a BRDF from interpolated data */
		} else {
			vec x(_data->dimX());
			params::convert(&cart[0], params::CARTESIAN, _data->input_parametrization(), &x[0]);
			
			vec y = _data->value(x);
			Spectrum res;
			if(_data->dimY() == 3) {
				res.fromLinearRGB(std::max(y[0], 0.0), std::max(y[1], 0.0), std::max(y[2], 0.0));
			} else {
				res = Spectrum(std::max(y[0], 0.0));
			}
			return res;
		}
	}

	virtual Spectrum sample(BSDFSamplingRecord &bRec, const Point2 &sample) const {
		Float pdf;
		return AltaBRDF::sample(bRec, pdf, sample);
	}

	virtual Spectrum sample(BSDFSamplingRecord &bRec, Float &pdf, const Point2 &sample) const {
		if (Frame::cosTheta(bRec.wi) <= 0)
			return Spectrum(0.0f);

#ifdef USE_VMF
		// Copy element to modify it
		Point2 vsample = sample;

		if(vsample.x < 0.5) {
			vsample.x *= 2;
			bRec.wo = warp::squareToCosineHemisphere(vsample);
			bRec.sampledComponent = 0;
			bRec.sampledType = EDiffuseReflection;
		} else {
			vsample.x = 2.0*(sample.x - 0.5);
			Vector R = reflect(bRec.wi);

			// Sample from a vmf lobe centered around (0, 0, 1)
			Vector localDir = _distrib.sample(vsample);

			// Rotate into the correct coordinate system
			bRec.wo = Frame(R).toWorld(localDir);
			bRec.sampledComponent = 1;
			bRec.sampledType = EGlossyReflection;
		}
#else
		bRec.wo = warp::squareToCosineHemisphere(sample);
		bRec.sampledComponent = 0;
		bRec.sampledType = EDiffuseReflection;
#endif
		bRec.eta = 1.0f;
		pdf = this->pdf(bRec, ESolidAngle);
		return getReflectance(bRec.wi, bRec.wo);

	}

	virtual Spectrum eval(const BSDFSamplingRecord &bRec, EMeasure measure = ESolidAngle) const {
		if (measure != ESolidAngle) {
			return Spectrum(0.0f);
		}

		return INV_PI * Frame::cosTheta(bRec.wo) * getReflectance(bRec.wi, bRec.wo);
	}

	virtual Float pdf(const BSDFSamplingRecord &bRec, EMeasure measure) const {
		if (measure != ESolidAngle
			|| Frame::cosTheta(bRec.wi) <= 0
			|| Frame::cosTheta(bRec.wo) <= 0)
			return 0.0f;

#ifdef USE_VMF
		Vector R     = reflect(bRec.wi);
		Float vmfPdf = _distrib.eval(dot(R, bRec.wo));

		return 0.5*vmfPdf + 0.5*warp::squareToCosineHemispherePdf(bRec.wo);
#else
		return warp::squareToCosineHemispherePdf(bRec.wo);
#endif
	}

	Float getRoughness(const Intersection &its, int component) const {
		return std::numeric_limits<Float>::infinity();
	}
	
	inline Vector reflect(const Vector &wi) const {
		return Vector(-wi.x, -wi.y, wi.z);
	}

	MTS_DECLARE_CLASS()

private:
	ptr<function> _func;
	ptr<data> _data;
#ifdef USE_VMF
	VonMisesFisherDistr _distrib;
#endif
};

MTS_IMPLEMENT_CLASS_S(AltaBRDF, false, BSDF)
MTS_EXPORT_PLUGIN(AltaBRDF, "ALTA BRDF evaluation inside MTS");
MTS_NAMESPACE_END

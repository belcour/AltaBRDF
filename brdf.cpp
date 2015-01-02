// MTS headers
#include <mitsuba/render/bsdf.h>
#include <mitsuba/hw/basicshader.h>
#include <mitsuba/core/warp.h>

// ALTA headers
#include <core/common.h>
#include <core/ptr.h>
#include <core/plugins_manager.h>
#include <core/function.h>

// STL
#include <iostream>

MTS_NAMESPACE_BEGIN

class AltaBRDF : public BSDF {
public:
	/* Constructors & destructors */

	AltaBRDF(const Properties& props) : BSDF(props), _data(NULL) {
		if(props.hasProperty("data-plugin")) {
			_data = plugins_manager::get_data(props.getString("data-plugin"));
			_data->load(props.getString("filename"));
			_func = NULL;
		} else {
			_func = plugins_manager::get_function(props.getString("filename"));
		}
	}

	AltaBRDF(Stream *stream, InstanceManager *manager) : BSDF(stream, manager), _data(NULL) {
		configure();
	}

	~AltaBRDF() {
		delete _func;
	}

	/* Mitsuba interface */

	virtual void configure() {	
		BSDF::configure();
	}
	
	virtual Spectrum sample(BSDFSamplingRecord &bRec, const Point2 &sample) const {
		if (Frame::cosTheta(bRec.wi) <= 0)
			return Spectrum(0.0f);

		bRec.wo = warp::squareToCosineHemisphere(sample);
		bRec.eta = 1.0f;
		bRec.sampledComponent = 0;
		bRec.sampledType = EDiffuseReflection;
		return eval(bRec);
	}

	virtual Spectrum sample(BSDFSamplingRecord &bRec, Float &pdf, const Point2 &sample) const {
		if (Frame::cosTheta(bRec.wi) <= 0)
			return Spectrum(0.0f);

		bRec.wo = warp::squareToCosineHemisphere(sample);
		bRec.eta = 1.0f;
		bRec.sampledComponent = 0;
		bRec.sampledType = EDiffuseReflection;
		pdf = warp::squareToCosineHemispherePdf(bRec.wo);
		return eval(bRec);

	}

	virtual Spectrum eval(const BSDFSamplingRecord &bRec, EMeasure measure = ESolidAngle) const {
		if (measure != ESolidAngle
			|| Frame::cosTheta(bRec.wi) <= 0
			|| Frame::cosTheta(bRec.wo) <= 0)
			return Spectrum(0.0f);

		double cart[6];
		cart[0] = bRec.wi[0];
		cart[1] = bRec.wi[1];
		cart[2] = bRec.wi[2];
		cart[3] = bRec.wo[0];
		cart[4] = bRec.wo[1];
		cart[5] = bRec.wo[2];

		/* Return the value of the BRDF from the function object */
		if(_func != NULL) {
			vec x(_func->dimX());
			params::convert(&cart[0], params::CARTESIAN, _func->input_parametrization(), &x[0]);
			vec y = _func->value(x);
			Spectrum res;
			if(_func->dimY() == 3) {
				res.fromSRGB(std::max(y[0], 0.0), std::max(y[1], 0.0), std::max(y[2], 0.0));
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
				res.fromSRGB(std::max(y[0], 0.0), std::max(y[1], 0.0), std::max(y[2], 0.0));
			} else {
				res = Spectrum(std::max(y[0], 0.0));
			}
			return res;
		}

	}

	virtual Float pdf(const BSDFSamplingRecord &bRec, EMeasure measure) const {
		if (measure != ESolidAngle
			|| Frame::cosTheta(bRec.wi) <= 0
			|| Frame::cosTheta(bRec.wo) <= 0)
			return 0.0f;

		return warp::squareToCosineHemispherePdf(bRec.wo);
	}

	MTS_DECLARE_CLASS()

private:
	function* _func;
	ptr<data> _data;
};

MTS_IMPLEMENT_CLASS_S(AltaBRDF, false, BSDF)
MTS_EXPORT_PLUGIN(AltaBRDF, "ALTA BRDF evaluation inside MTS");
MTS_NAMESPACE_END

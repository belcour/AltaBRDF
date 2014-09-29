AltaBRDF
========

[Mitsuba][] plugin to load [ALTA][] BRDFs and perform rendering of fitted BRDFs.

    [Mitsuba](http://http://mitsuba-renderer.org/)
    [ALTA](http://alta.gforge.inria.fr/)


## Usage

Once this shared library is compiled and added to Mitsuba plugin set, you can use any fitted BRDF from the ALTA library within Mitsuba with the following code in any Mitsuba XML file:

    <bsdf type="alta_brdf">
       <string name="filename" value="PATH-TO-FILE" />
    </bsdf>


## Note

The ALTA library does not support importance sampling yet. This plugin is provided without any IS support and uses the Lambertian importance function defined by Mitsuba. Consequenlty, this tool should not be used when seeking performances.

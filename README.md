AltaBRDF
========

[Mitsuba](http://http://mitsuba-renderer.org/) plugin to load [ALTA](http://alta.gforge.inria.fr/) BRDFs and perform rendering of fitted BRDFs.


## Usage

Once this shared library is compiled and added to Mitsuba plugin set, you can use any fitted BRDF from the ALTA library within Mitsuba with the following code in any Mitsuba XML file:

    <bsdf type="alta_brdf">
       <string name="filename" value="PATH-TO-FILE" />
    </bsdf>


## Example


<table>
<tr>
<td><img width="400px" src="https://raw.githubusercontent.com/belcour/AltaBRDF/master/matpreview-merl.png"/></td>
<td><img width="400px" src="https://raw.githubusercontent.com/belcour/AltaBRDF/master/matpreview-alta.png"/></td>
</tr>
<tr>
<td>MERL binary file</td>
<td>Fitted Blinn-Phong lobe</td>
</tr>
</table>


## Note

The ALTA library does not support importance sampling yet. This plugin is provided without any IS support and uses the Lambertian importance function defined by Mitsuba. Consequenlty, this tool should not be used when seeking performances.

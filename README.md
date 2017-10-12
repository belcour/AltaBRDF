AltaBRDF
========

[Mitsuba](http://http://mitsuba-renderer.org/) plugin to load [ALTA](http://alta.gforge.inria.fr/) BRDFs and perform rendering of fitted BRDFs.

*Note:* This BRDF plugin for Mitsuba is created using the head of the master branch of ALTA. It will probably not compile with ALTA `v0.2`.

## Build

To build this package, you can SCons or CMake. For example, using CMake:

    $ mkdir build
    $ cd build
    $ cmake -DEIGEN3_INCLUDE_DIR=/usr/include/eigen3 ..
    $ make && make install

The 'install' flag, will automatically move the resulting library to the Mitsuba plugin directory so that it is readily usable. Make sure that both Mitsuba and ALTA are available in the environment variables (you can do this by sourcing the `setpath.sh` or both softwares).

## Usage

Once this shared library is compiled and added to Mitsuba plugin set, you can use any fitted BRDF from the ALTA library within Mitsuba with the following code in any Mitsuba XML file:

    <bsdf type="alta_brdf">
       <string name="filename" value="PATH-TO-FILE" />
    </bsdf>

We provide xml scene files examples in `scenes`.

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

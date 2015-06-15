AltaBRDF
========

[Mitsuba](http://http://mitsuba-renderer.org/) plugin to load [ALTA](http://alta.gforge.inria.fr/) BRDFs and perform rendering of fitted BRDFs.


## Usage

Once this shared library is compiled and added to Mitsuba plugin set, you can use any fitted BRDF from the ALTA library within Mitsuba with the following code in any Mitsuba XML file:

    <bsdf type="alta_brdf">
       <string name="filename" value="PATH-TO-FILE" />
    </bsdf>


<script type="text/javascript">
function TwoFace(id, width, height) {
    if (!(this instanceof TwoFace)) {
        return new TwoFace(id, width, height);
    }

    var canvas = document.createElement('canvas'),
        container = document.getElementById(id),
        divide = 0.5;

    this.ctx = canvas.getContext('2d');
    this.images = [];
	 this.texts  = [];

    // Event handlers
    canvas.addEventListener('mousemove', handler, false);
    canvas.addEventListener('mousedown', handler, false);
    canvas.addEventListener('mouseup', handler, false);

    var self = this;

    function handler(ev) {
        if (ev.layerX || ev.layerX == 0) { // Firefox
            ev._x = ev.layerX;
            ev._y = ev.layerY;
        } else if (ev.offsetX || ev.offsetX == 0) { // Opera
            ev._x = ev.offsetX;
            ev._y = ev.offsetY;
        }

        var eventHandler = self[ev.type];
        if (typeof eventHandler == 'function') {
            eventHandler.call(self, ev);
        }
    }

    // Draw canvas into its container
    canvas.setAttribute('width', width);
    canvas.setAttribute('height', height);
    container.appendChild(canvas);

    Object.defineProperty(this, 'ready', {
        get: function () {
            return this.images.length >= 2;
        }
    });

    Object.defineProperty(this, 'width', {
        get: function () {
            return width;
        }
    });

    Object.defineProperty(this, 'height', {
        get: function () {
            return height;
        }
    });

    Object.defineProperty(this, 'divide', {
        get: function () {
            return divide;
        },
        set: function (value) {
            if (value > 1) {
                value = (value / 100);
            }

            divide = value;
            this.draw();
        }
    });
}




TwoFace.prototype = {
    add: function (src, text='Hello world') {
        var img = createImage(src, onload.bind(this));

        function onload(event) {
            this.images.push(img);
				this.texts.push(text);

            if (this.ready) {
                this.draw();
            }
        }
    },

    draw: function () {
        if (!this.ready) {
            return;
        }

        var lastIndex = this.images.length - 1,
            before = this.images[lastIndex - 1],
            after = this.images[lastIndex];

        this.drawImages(this.ctx, before, after);
        this.drawHandle(this.ctx);
		  
		  var text = this.texts[lastIndex - 1];
		  var text_width = this.ctx.measureText(text).width;

		  var split = this.divide * this.width;
		  this.ctx.fillText(this.texts[lastIndex], split+10, this.height-10);
		  this.ctx.fillText(text, split-10-text_width, this.height-10);
    },

    drawImages: function (ctx, before, after) {
        var split = this.divide * this.width;

        ctx.drawImage(after, 0, 0);
        ctx.drawImage(before, 0, 0, split, this.height, 0, 0, split, this.height);
    },

    drawHandle: function (ctx) {
        var split = this.divide * this.width;

        ctx.fillStyle = "rgb(220, 50, 50)";
        ctx.fillRect(split - 1, 0, 2, this.height);
    },

    mousedown: function (event) {
        var divide = event._x / this.width;
        this.divide = divide;

        this.dragstart = true;
    },

    mousemove: function (event) {
        if (this.dragstart === true) {
            var divide = event._x / this.width;
            this.divide = divide;
        }
    },

    mouseup: function (event) {
        var divide = event._x / this.width;
        this.divide = divide;

        this.dragstart = false;
    }
};




function createImage(src, onload) {
    var img = document.createElement('img');
    img.src = src;

    if (typeof onload == 'function') {
        img.addEventListener('load', onload);
    }

    return img;
}

</script>

  
<figure>
    <div id="twoface-demo"></div>
    <figcaption></figcaption>
</figure>


<script type="text/javascript">
var twoface = TwoFace('twoface-demo', 683, 512);
twoface.add('matpreview-merl.png', 'MERL');
twoface.add('matpreview-alta.png', 'ALTA, Blinn BRDF');
</script>

## Note

The ALTA library does not support importance sampling yet. This plugin is provided without any IS support and uses the Lambertian importance function defined by Mitsuba. Consequenlty, this tool should not be used when seeking performances.

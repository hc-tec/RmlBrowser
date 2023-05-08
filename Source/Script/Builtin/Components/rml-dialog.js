

rml = `
<div 
    r-class="class_"
    id="__rue__"
    r-click="close">
    <div class="rml-dialog" r-click="StopPropagation">
        <div class="rml-dialog-header">
            <slot name="header" />
        </div>  
        <div class="rml-dialog-content">
            <slot />
        </div>
        <div class="rml-dialog-footer">
            <slot name="footer" />
        </div>
        <div class="rml-dialog-close">
            <img 
                src="//home/titto/CProjects/RmlUi5.0/Extensions/DevTools/static/imgs/close.png"
                 r-click="close"/>
        </div>
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: 'rml-dialog-wrapper close',
    },
    methods: {
        open() {
            this.class_ = 'rml-dialog-wrapper open'
        },
        close() {
            this.class_ = 'rml-dialog-wrapper close'
        },
        StopPropagation(el, ev) {
            ev.stopPropagation()
        }
    },
    onMounted() {
        if (this.$attrs.type) {
            this.class_ += ' ' + this.$attrs.type
        }
    }
})
`.slice(9)

style = `
<style>
.rml-dialog-wrapper {
    position: fixed;
    left: 0;
    top: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0,0,0,60);
    overflow: hidden auto;
    padding-top: 80px;
    padding-bottom: 80px;
    transition: all .2s;
}
.rml-dialog-wrapper.open {
    z-index: 10;
    opacity: 1;
}
.rml-dialog-wrapper.close {
    z-index: -1;
    opacity: 0;
}
.rml-dialog {
    top: 10%;
    left: 50%;
    transform: translateX(-50%);
    position: relative;
    width: 400px;
    background-color: #fff;
    color: rgb(44,62,80);    
    border-radius: 20px;
}
.rml-dialog-close {
    position: absolute;
    top: -10px;
    right: -10px;
    width: 20px;
    background-color: #fff;
    border-radius: 12px;
    padding: 7px;
    transition: transform .2s;
}
.rml-dialog-close:hover {
    transform: translate(-3px, 3px);
}
.rml-dialog-header {
    text-align: center;
    padding: 10px 16px;
}
.rml-dialog-content {
    padding: 10px 16px;
    width: 100%;
}
.rml-dialog-footer {
    text-align: center;
    padding: 0 16px 10px;
}
`.slice(8)

component = new CustomComponent('rml-dialog')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










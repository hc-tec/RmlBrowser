

rml = `
<div 
    id="__rue__"
    r-class="class_">
    <div class="rml-notification-content">
        <div class="rml-notification-content-header">
            <h4>
                <slot name="header" />
            </h4>
        </div>
        <div class="rml-notification-content-text">
            <slot />
        </div>
    </div>
    <div class="rml-notification-close">
        <img 
            src="//home/titto/CProjects/RmlUi5.0/Extensions/DevTools/static/imgs/close.png"
             r-click="close"/>
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: 'rml-notification primary flat close',
    },
    methods: {
        close() {
            this.class_ = 'rml-notification primary flat close'
        },
        open() {
            this.class_ = 'rml-notification primary flat open'
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
.rml-notification {
    position: fixed;
    right: 10px;
    top: 0;
    z-index: 110;
    background: #fff;
    color: rgb(44,62,80);
    width: 340px;
    border-radius: 15px;
    margin: 3px 10px;
    transition: background-color .2s, transform .3s;
}
.rml-notification.close {
    transform: translateX(400px);
}
.rml-notification.open {
    transform: translateX(0);
}
.rml-notification-content {
    padding: 20px;
}
.rml-notification-content-header {
    padding: 0 0 6px;
}
.rml-notification-content-text {
    font-size: .75rem;
    line-height: 1rem;
}
.rml-notification-close {
    width: 20px;
    position: absolute;
    border: 0;
    top: 5px;
    right: 5px;
}
.rml-notification-close img {
    
}
`.slice(8)

component = new CustomComponent('rml-notification')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










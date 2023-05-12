

rml = `
<div 
    id="__rue__"
    class="rml-input">
    <input r-class="class_" r-type="type" />
    <div r-if="iconVisible" 
        class="rml-input-icon">
        <img r-src="icon" />
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: '',
        icon: '',
        type: 'text',
        iconVisible: false,
        inputEl: null
    },
    methods: {
        getValue() {
            return this.inputEl.value
        },
        setValue(value) {
            this.inputEl.value = value
        }
    },
    onMounted() {
        this.inputEl = dom.convertToElementFormControl(this.$el.getElementsByTagName('input')[0])
        if (this.$attrs.type) {
            this.type = this.$attrs.type 
        }
        if (this.$attrs.width) {
            this.inputEl.setProperty('width', this.$attrs.width)
        } else {
            this.inputEl.setProperty('width', '200px')
        }
        if (this.$attrs.icon) {
            this.iconVisible = true
            this.class_ += ' has-icon'
            this.icon = this.$attrs.icon
        }
    }
})
`.slice(9)

style = `
<style>
.rml-input {
    display: flex;
    align-items: center;
    justify-content: flex-start;
    position: relative;
    border-radius: 12px;
}
.rml-input input {
    background-color: rgb(244,247,248);
    color: rgb(44,62,80);
    border-width: 2px;
    border-color: rgb(244,247,248);
    padding: 10px 13px 10px 10px;
    border-radius: 12px;
    font-size: 16px;
    line-height: 1;
    transition: all .2s;
}
.rml-input input:focus {
    background-color: rgb(240,243,244);
    padding-left: 15px;
}
.rml-input input.has-icon {
    padding-left: 38px;
}
.rml-input input.has-icon:focus {
    padding-left: 43px;
}
.rml-input-icon {
    position: absolute;
    background-color: rgb(244,247,248);
    left: 10px;
    width: 23px;
    height: 23px;
    border-radius: 12px;
    top: 50%;
    transform: translateY(-50%);
}
.rml-input-icon img {
    width: 100%;
}
`.slice(8)

component = new CustomComponent('rml-input')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();
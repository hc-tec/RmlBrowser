

rml = `
<div 
    id="__rue__"
    class="rml-select">
    <input />
    <div class="rml-options">
        <div 
            class="rml-option"
            r-for="option in options"
            r-value="option"
            r-click="handleClick">
            {{ option }}
        </div>
    </div>
    <div class="rml-select-icon">
        <img src="//home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/imgs/down_arrow.png" />
    </div>
    <div class="rml-option-value">
        {{ value }}
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        options: ['so?', '456'],
        value: 'default',
    },
    methods: {
        handleClick(e) {
            this.value = e.getAttribute('value')
        },
        getValue() {
            return this.value
        },
        setOptions(options) {
            this.options = options
        },
        setValue(value) {
            this.value = value
        }
    },
    onMounted() {
        if (this.$attrs.type) {
            this.type = this.$attrs.type 
        }
        // this.options = ['123', '456', '789','q', 'w', 'e']
    }
})
`.slice(9)

style = `
<style>
.rml-select {
    position: relative;
    min-height: 38px;
    width: 200px;
    display: flex;
    justify-content: center;
    align-items: center;
    background-color: rgb(244,247,248);
    color: rgb(44,62,80);
    border-radius: 12px;
    transition: background .2s;
    font-size: 16px;
}
.rml-select:hover {
    background-color: rgb(240,243,244);
}
.rml-select input {
    opacity: 0;
    border-radius: 12px;
    cursor: pointer;
    padding: 7px 30px 7px 13px;
    width: 100%;
}
.rml-option-value {
    position: absolute;
    left: 20px;
    top: 50%;
    transform: translateY(-50%);
}
.rml-select-icon {
    position: absolute;
    right: 10px;
    top: 50%;
    transform: translateY(-50%);
    width: 16px;
    transition: transform .2s;
}
.rml-select-icon img {
    width: 100%;
}
.rml-select input:focus + .rml-options + .rml-select-icon {
    transform: translateY(-50%) rotate(180deg);
}
.rml-options {
    opacity: 0;
    z-index: -1;
    position: absolute;
    left: 0;
    top: 104%;
    width: 200px;
    background-color: rgb(244,247,248);
    padding: 5px 0;
    border-radius: 0 0 12px 12px;
    height: 200px;
    overflow: visible;
    transition: all .2s;
}
.rml-option {
    padding: 4px 15px;
    text-align: left;
    margin: 2px 0;
    max-height: 40px;
    transition: background-color .1s, transform .1s;
    border-radius: 5px;
}
.rml-option:hover {
    transform: translateX(10px);
    color: rgb(${RUE_PRIMARY});
}
.rml-select input:focus + .rml-options {
    opacity: 1;
    z-index: 10;
}
`.slice(8)

component = new CustomComponent('rml-select')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();
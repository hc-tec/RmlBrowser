

rml = `
<div 
    id="__rue__"
    r-class="class_">
    
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: 'rml-dialog',
    },
    methods: {
        
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
.rml-dialog {

}
`.slice(8)

component = new CustomComponent('rml-dialog')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










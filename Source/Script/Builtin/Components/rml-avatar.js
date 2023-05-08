

rml = `
<div 
    id="__rue__"
    class="rml-avatar">
    <slot />
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        icon: '',
    },
    methods: {
        
    },
    onMounted() {
        
    }
})
`.slice(9)

style = `
<style>
.rml-avatar {
    background-color: rgb(244,247,248);
    display: flex;
    justify-content: center;
    align-items: center;
    border-radius: 15px;
    width: 44px;
    height: 44px;
    transition: background-color .1s;
}
.rml-avatar:hover {
    background-color: rgb(240,243,244);
}
.rml-avatar img {
    width: 25px;
    image-color: rgb(44,62,80);
}
`.slice(8)

component = new CustomComponent('rml-avatar')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();
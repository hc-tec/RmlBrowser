

rml = `
<div 
    id="__rue__"
    r-class="class_">
    <rml-alert type="warn">
        <template slot="title">It is title</template>
        <template>
            <rml-button>
                in my page
            </rml-button>
        </template>
    </rml-alert>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: '',
    },
    methods: {
        
    },
    onMounted() {

    }
})
`.slice(9)

style = `
<style>
`.slice(8)

component = new CustomComponent('my-page')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










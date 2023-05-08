

rml = `
<div 
    id="__rue__"
    r-class="class_">
    <div class="rml-alert-title">
        <slot name="title" />
    </div>
    <div class="rml-alert-content">
        <slot />
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: 'rml-alert flat primary',
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
.rml-alert {
    padding: .1rem 1rem;
    border-left-width: 1px;
    margin: 1rem 0;
    position: relative;
    border-radius: 0 10px 10px 0;
}
.rml-alert.primary {
    border-color: rgb(${RUE_PRIMARY});
}
.rml-alert.primary:hover {
    background: rgba(${RUE_PRIMARY},${RUE_BG_ALPHA});
    color: rgb(${RUE_PRIMARY});
}
.rml-alert.success {
    border-color: rgb(${RUE_SUCCESS});
}
.rml-alert.success:hover {
    background: rgba(${RUE_SUCCESS},${RUE_BG_ALPHA});
    color: rgb(${RUE_SUCCESS});
}
.rml-alert.danger {
    border-color: rgb(${RUE_DANGER});
}
.rml-alert.danger:hover {
    background: rgba(${RUE_DANGER},${RUE_BG_ALPHA});
    color: rgb(${RUE_DANGER});
}
.rml-alert.warn {
    border-color: rgb(${RUE_WARN});
}
.rml-alert.warn:hover {
    background: rgba(${RUE_WARN},${RUE_BG_ALPHA});
    color: rgb(${RUE_WARN});
}
.rml-alert-title {
    font-size: .85rem;
    margin: 1rem 0;
    padding: 0;
}
.primary .rml-alert-title {
    color: rgb(${RUE_PRIMARY});
}
.success .rml-alert-title {
    color: rgb(${RUE_SUCCESS});
}
.danger .rml-alert-title {
    color: rgb(${RUE_DANGER});
}
.warn .rml-alert-title {
    color: rgb(${RUE_WARN});
}
.rml-alert-content {
    font-size: .85rem;
    margin: 1rem 0;
    padding: 0;
    opacity: .7;
}
`.slice(8)

component = new CustomComponent('rml-alert')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










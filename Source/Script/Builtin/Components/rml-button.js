

rml = `
<div 
    id="__rue__"
    r-class="class_">
    <slot />
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        class_: 'rml-button primary',
    },
    methods: {
        
    },
    onMounted() {
        if (this.$attrs.type) {
            this.class_ += ' ' + this.$attrs.type
        }
        if (this.$attrs.flat) {
            this.class_ += ' flat'
        }
    }
})
`.slice(9)

style = `
<style>
.rml-button {
    display: inline-block;
    cursor: pointer;
    position: relative;
    border-radius: 12px;
    padding: 6px 15px;
    font-size: 16px;
    transition: background .2s;
}
.primary {
    background: rgb(${RUE_PRIMARY});
    color: #fff;
}
.primary.flat {
    background: rgba(${RUE_PRIMARY},${RUE_BG_ALPHA});
    color: rgb(${RUE_PRIMARY});
}
.primary.flat:hover {
    background: rgb(${RUE_PRIMARY});
    color: #fff;
}
.success {
    background: rgb(${RUE_SUCCESS});
    color: #fff;
}
.success.flat {
    background: rgba(${RUE_SUCCESS},${RUE_BG_ALPHA});
    color: rgb(${RUE_SUCCESS});
}
.success.flat:hover {
    background: rgb(${RUE_SUCCESS});
    color: #fff;
}
.danger {
    background: rgb(${RUE_DANGER});
    color: #fff;
}
.danger.flat {
    background: rgba(${RUE_DANGER},${RUE_BG_ALPHA});
    color: rgb(${RUE_DANGER});
}
.danger.flat:hover {
    background: rgb(${RUE_DANGER});
    color: #fff;
}
.warn {
    background: rgb(${RUE_WARN});
    color: #fff;
}
.warn.flat {
    background: rgba(${RUE_WARN},${RUE_BG_ALPHA});
    color: rgb(${RUE_WARN});
}
.warn.flat:hover {
    background: rgb(${RUE_WARN});
    color: #fff;
}
.rml-button:hover {
    opacity: 0.8;
}
.rml-button:active {
    opacity: 0.7;
}
`.slice(8)

component = new CustomComponent('rml-button')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();
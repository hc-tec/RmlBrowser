

rml = `
<div 
    id="__rue__"
    class="elements-wrapper">
    <div class="elements-container">
    
    </div>
    <div class="elements-info">
<!--        <div class="inherit-prop">-->
<!--            <p>Inherit from #app</p>-->
<!--            <div class="element-prop">-->
<!--                <span>body</span>-->
<!--                <span>{</span>-->
<!--                <div class="unit-prop">-->
<!--                    <div r-for="prop in properties">-->
<!--                        <span class="prop-key">{{ prop.key }}&nbsp;</span>-->
<!--                        <span>:&nbsp;</span>-->
<!--                        <span class="prop-value">{{ prop.value }}</span>-->
<!--                    </div>-->
<!--                </div>-->
<!--                <span>}</span>-->
<!--            </div>-->
<!--        </div>-->
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        host: null,
        documents: {},
        elStack: [],
        ruleName: '',
        properties: [],
    },
    methods: {
       parseDocument(root, index, obj) {
            const tag = root.getTagName()
            if (tag === '#text') {
                root = dom.convertToElementText(root)
                obj.start = root.text
                return
            }
            let rml = '&lt;' + tag
            const attrs = root.getAttributes()
            Object.keys(attrs).forEach(key => {
                rml += ' '
                rml += key
                rml += '=\"'
                rml += attrs[key]
                rml += '\"'
            })
            rml += '&gt;'
            obj.start = rml
            obj.path = this.elStack.join('-')
            const childNum = root.getNumChildren(false)
            obj.children = []
            for(let i = 0; i < childNum; i++) {
                obj.children.push({})
                this.elStack.push(i)
                this.parseDocument(root.getChild(i), i, obj.children[i])
                this.elStack.pop()  
            }
            rml = '&lt;/'
            rml += tag
            rml += '&gt;'
            obj.end = rml
       },
       toElements(el, obj) {
           el.setAttribute('path', obj.path)
           let p = document.createElement('p')
           p.innerRML = obj.start
           el.appendChild(p, true)
           if (obj.children) {
               document.addEventListener(p, 'click', e => {
                    this.showElementInfo(e)
               })
               const children = document.createElement('div')
               children.setClassNames('children children-hidden')
               el.appendChild(children, true)
               obj.children.forEach(child => {
                   const div = document.createElement('div')
                   div.setClassNames('element')
                   children.appendChild(div, true)
                   this.toElements(div, child)
               })
           }
           if (obj.end) {
               let p = document.createElement('p')
               p.setClassNames('omit')
               p.innerRML = '&nbsp;&nbsp;...'
               el.appendChild(p, true)
               if (obj.children) {
                   document.addEventListener(p, 'click', e => {
                        this.displayChildren(e)
                   })
               }
               p = document.createElement('p')
               p.innerRML = obj.end
               el.appendChild(p, true)
           }
       },
       showElementInfo(e) {
            const el = e.getTargetElement().getParentNode()
            if (el.getTagName() !== 'div') return
            const source = this.findElementByPath(el)
            source.scrollIntoView(true)
            rml.tab.showBox(source) 
            const properties = rml.dom.getElementProperties(source)
            // log(JSON.stringify(properties))
            // this.properties = []
            // properties[0].list[0].list.forEach((prop,i) => {
            //     this.properties.push({key: prop[0], value: dom.getPropString(prop[1])})
            // })
            // this.properties = this.properties
            this.showCssProperties(properties)
            e.stopPropagation()
       },
       showCssProperties(properties) {
           const info = this.$el.getElementsByClassName('elements-info')[0]
           info.innerRML = ''
           properties.forEach(inherit => {
               let inheritProp = document.createElement('div')
               inheritProp.setClassNames('inherit-prop')
               info.appendChild(inheritProp, true)
               if (inherit.inherit) {
                   const p = document.createElement('p')
                   p.innerRML = 'inherit from ' + inherit.inherit
                   inheritProp.appendChild(p, true) 
               }
               inherit.list.forEach(elProp => {
                   let elmentProp = document.createElement('div')
                   elmentProp.setClassNames('element-prop')
                   inheritProp.appendChild(elmentProp, true)
                   let span = document.createElement('span')
                   span.innerRML = Object.keys(elProp.source).length ? elProp.source.rule : 'element.style'
                   elmentProp.appendChild(span, true)
                   span = document.createElement('span')
                   span.innerRML = '{'
                   elmentProp.appendChild(span, true)
                   
                   let unitProp = document.createElement('div')
                   unitProp.setClassNames('unit-prop')
                   elmentProp.appendChild(unitProp, true)
                   
                   elProp.list.forEach(prop => {
                       let propEl = document.createElement('div')
                        unitProp.appendChild(propEl, true)
                        let span = document.createElement('span')
                        span.innerRML = prop[0] + '&nbsp;'
                        span.setClassNames('prop-key')
                        propEl.appendChild(span, true)
                        span = document.createElement('span')
                        span.innerRML = ':&nbsp;'
                        propEl.appendChild(span, true)
                        span = document.createElement('span')
                        span.setClassNames('prop-value')
                        span.innerRML = dom.getPropString(prop[1])
                        propEl.appendChild(span, true)
                   })
                   span = document.createElement('span')
                   span.innerRML = '}'
                   elmentProp.appendChild(span, true)
               })
           })
       },
       displayChildren(e) {
            const el = e.getTargetElement().getParentNode()
            if (el.getTagName() !== 'div') return
            const children = el.getChild(1)
            const classNames = children.getClassNames()
            if (classNames === 'children children-hidden') {
                children.setClassNames('children children-visible')
            } else {
                children.setClassNames('children children-hidden')
            }
            e.stopPropagation()
       },
       findElementByPath(el) {
            let res = this.host
            const path = el.getAttribute('path')
            if (path === '') return res
            const path_list = path.split('-')
            path_list.forEach(i => {
                res = res.getChild(i)
            })
            return res
       }
    },
    onMounted() {

    }
})

rml.extension.onClick = (tab) => {
    if (document.isVisible()) {
        document.hide()
        rml.tab.hideBox()
    } else {
        document.show(0, 0)
   
        const host = rml.tab.getHostDocument()
        if (!host) return
        if (__rue__.host) return;
        __rue__.host = host
        __rue__.parseDocument(host, '', __rue__.documents)
        log('document')
        log(JSON.stringify(__rue__.documents))
        const div = document.createElement('div')
        div.setClassNames('element')
        __rue__.$el.getElementsByClassName('elements-container')[0].appendChild(div, true)
        __rue__.toElements(div, __rue__.documents)
    }
}

`.slice(9)

style = `
<style>
.elements-wrapper {
    color: rgb(44,62,80);
    display: flex;
    font-size: 14px;
    height: 100%;
}
.elements-container {
    margin-top: 10px;
    line-height: 1.2;
    width: 70%;
}
.element {
    padding-left: 10px;
}
.element p {
    word-break: break-all;
}
.element p:hover {
    background-color: rgb(243,249,255);
}
.children-visible {
    display: block;
}
.children-visible + .omit {
    display: none;
}
.children-hidden {
    display: none;
}
.children-hidden + .omit {
    display: block;
}
.elements-info {
    width: 30%;
    border-left-width: 1px;
    border-left-color: rgb(245,245,245);
}
.inherit-prop > p {
    background-color: rgb(245,245,245);
    border-width: 1px;
    border-top-color: rgb(234,234,234);
    padding-left: 5px;
    border-bottom-color: rgb(234,234,234);
    word-break: break-all;
}
.element-prop {
    padding: 5px;
    border-width: 1px;
    border-top-color: rgb(234,234,234);
    border-bottom-color: rgb(234,234,234);
}
.element-prop > span:first-child {
    color: rgb(158,51,164);
    margin-right: 5px;
}
.unit-prop {
    padding: 5px 0 5px 10px;
}
`.slice(8)

component = new CustomComponent('elements')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










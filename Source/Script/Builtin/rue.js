const isPropAccess = (s) => {
    return s !== s.split('.')[0]
}

class RueRefs {
    constructor() {
        this.$refs = {}
    }
    addRef(id, rue) {
        this.$refs[id] = rue
    }
    findRef(id) {
        return this.$refs[id]
    }
}

const rueRefs = new RueRefs()

class Rue {
    constructor(options) {
        rueRefs.addRef(options.el, this)
        this.$options = options
        this.$el = this.getBindingElement()
        this.$refs = {}
        this.$attrs = {}
        this.deps = {}
        this.rue = this.$options.data
        this._rue = {}
        this.compileOptions()
        this._defineProp()
        this.proxyData()
        this.init()
    }

    getRef(name) {
        return rueRefs.findRef(this.$refs[name])
    }

    getBindingElement() {
        return document.getElementById(this.$options.el)
    }

    compileOptions() {
        Object.keys(this.rue).forEach(key => {
            this.deps[key] = []
        })
        this.parse(this.$el, -1)
        log(JSON.stringify(this.deps))
    }

    parse(root) {
        if (root === null) return
        if (root.getId().startsWith('rue') && root !== this.$el) {
            const name = root.getParentNode().getAttribute('ref')
            if (name) this.$refs[name] = root.getId()
            return;
        }
        if (root.getId().startsWith('rue')) {
            const p = root.getParentNode();
            const attrs = p.getAttributes()
            Object.keys(attrs).forEach(key => {
                this.$attrs[key] = attrs[key]
            })
        }
        let index = arguments[1]
        if (index === undefined) index = -1
        if (root.getTagName() === '#text') {
            root = dom.convertToElementText(root)
            let rml = root.text
            const p = root.getParentNode()
            if (!p.hasAttribute('origin_rml')) {
                p.setAttribute('origin_rml', rml)
            } else {
                rml = p.getAttribute('origin_rml')
            }
            const regex = /{{\s*(\w+(?:\.\w+)*)\s*}}/g
            let match;
            while ((match = regex.exec(rml)) !== null) {
                let val = match[1];
                const origin_val = val
                if(isPropAccess(val)) {
                    val = val.split('.')[0]
                }
                if (index >= 0) {
                    const item = `${val}__${index}`
                    root.setAttribute('r_i', index)
                    // rml = rml.replace(`{{ ${val} }}`, `{{ ${item} }}`)
                    this.deps[item].push(['text', root, rml, origin_val])
                    continue
                }
                if (this.deps[val] === undefined) {
                    const item = `${val}__0`
                    root.setAttribute('r_i', 0)
                    // rml = rml.replace(`{{ ${val} }}`, `{{ ${item} }}`)
                    if (this.deps[item] === undefined) this.deps[item] = []
                    this.rue[item] = ''
                    this.defineProp(this._rue, item, '')
                    this.deps[item].push(['text', root, rml, origin_val])
                    continue
                }
                this.deps[val].push(['text', root, rml, origin_val])
            }
            return
        } else {
            const attrs = root.getAttributes()
            Object.keys(attrs).forEach(key => {
                if (!key.startsWith('r-')) return
                const instruct = key.split('-')[1]
                let val = attrs[key]
                if (instruct === 'for') {
                    const for_ = val.split(' in ')
                    const item_v = for_[0]
                        , arr_v = for_[1]
                    root.removeAttribute('r-for')
                    this.deps[arr_v].push([instruct, root, item_v])
                } else if (instruct === 'click') {
                    const event_handler = this.$options.methods[val]
                    root.addEventListener(root, 'click', e => {
                        event_handler.apply(this, [root, e])
                    })
                } else if (instruct === 'load') {

                } else {
                    const origin_val = val
                    if(isPropAccess(val)) {
                        val = val.split('.')[0]
                    }
                    if (index >= 0) {
                        let item = `${val}__${index}`
                        if (this.$options.methods[val] !== undefined) {
                            item = val
                        }
                        root.setAttribute('r_i', index)
                        this.deps[item].push([instruct, root, origin_val])
                        return
                    }
                    if (this.deps[val] === undefined) {
                        let item = `${val}__0`
                        if (this.$options.methods[val] !== undefined) {
                            item = val
                        }
                        root.setAttribute('r_i', 0)
                        if (this.deps[item] === undefined) this.deps[item] = []
                        this.rue[item] = ''
                        this.defineProp(this._rue, item, '')
                        this.deps[item].push([instruct, root, origin_val])
                        return
                    }
                    this.deps[val].push([instruct, root, origin_val])
                }
            })
        }
        const child_num = root.getNumChildren(false)
        for (let i = 0; i < child_num; ++i) {
            const child = root.getChild(i)
            this.parse(child, index)
        }
    }

    _defineProp() {
        Object.keys(this.rue).forEach(key => {
            this.defineProp(this._rue, key, this.rue[key])
        })
    }

    bindMethods(key, val) {

    }


    triggerUpdate(vary, new_val) {
        log(`triggerUpdate ${vary} ${new_val}`)
        const dep_arr = this.deps[vary]
        dep_arr.forEach(dep => {
            const instruct = dep[0]
            const el = dep[1]
            switch (instruct) {
                case 'if':
                    if (new_val) {
                        el.setProperty('display', 'block')
                    } else {
                        el.setProperty('display', 'none')
                    }
                    break
                case 'text':
                    let origin = dep[2]
                    let index = el.getAttribute('r_i')
                    if (index !== '') {
                        vary = vary.split('__')[0]
                    }
                    let origin_attr = dep[3]
                    let regex = `{{ ${origin_attr} }}`
                    let real_val = new_val
                    if (isPropAccess(origin_attr)) {
                        if (typeof real_val !== 'object') break
                        origin_attr.split('.').forEach((key, index) => {
                            if (index === 0) return
                            if (real_val === undefined) return
                            real_val = real_val[key]
                        })
                    }
                    el.text = origin.replace(regex, real_val)
                    break
                case 'for':
                    const len = new_val.length
                    const item_v = dep[2]
                    const parent = el.getParentNode()

                    // clear
                    const child_n = parent.getNumChildren(false)
                    log(`clear ${child_n}`)
                    let n = child_n - 1;
                    while (n--) {
                        const rmv_i = parent.getNumChildren(false) - 1
                        log(`clear child ${parent.getChild(rmv_i).innerRML}`)
                        parent.removeChild(parent.getChild(rmv_i))
                    }
                    if (child_n > 1) {
                        log(parent.innerRML)
                        // break
                    }
                    if (len === 0) break
                    // new data
                    el.setAttribute('r_loaded', '0')
                    for (let i = 1; i < len; ++i) {
                        const item = `${item_v}__${i}`
                        this.rue[item] = new_val[i]
                        this.deps[item] = []
                        this.defineProp(this._rue, item, '')
                        const copy = el.clone()
                        parent.appendChild(copy, true)
                        this.parse(copy, i)
                        this._rue[item] = new_val[i]
                        this.elementLoad(copy)
                    }
                    this._rue[`${item_v}__0`] = new_val[0]
                    this.elementLoad(el)
                    log(`${parent.innerRML}`)
                    log(JSON.stringify(this.deps))
                    break
                case 'click':

                    break
                case 'load':
                    break
                case 'class':
                    el.setClassNames(new_val)
                    break
                default:
                    const attr = instruct
                    const origin1 = dep[2]
                    const index1 = el.getAttribute('r_i')
                    if (index1 !== '') {
                        vary = vary.split('__')[0]
                    }
                    let real_val1 = new_val
                    if (isPropAccess(origin1)) {
                        if (typeof real_val1 !== 'object') break
                        origin1.split('.').forEach((key, index) => {
                            if (index === 0) return
                            real_val1 = real_val1[key]
                        })
                    }
                    el.setAttribute(attr, real_val1)
            }
        })
    }

    defineProp(obj, key, value) {
        Object.defineProperty(obj, key, {
            configurable:true,
            enumerable:true,
            get(){
                return value
            },
            set: (val) => {
                value = val
                try {
                    this.triggerUpdate(key, val)
                } catch (e) {
                    log(e)
                }
            }
        })
    }

    proxyData() {
        Object.keys(this._rue).forEach(key => {
            Object.defineProperty(this, key, {
                configurable:true,
                enumerable:true,
                get(){
                    return this._rue[key]
                },
                set(val){
                    this._rue[key] = val
                }
            })
        })
        if (this.$options.methods) {
            Object.keys(this.$options.methods).forEach(key => {
                this[key] = this.$options.methods[key]
            })
        }
    }

    init() {
        Object.keys(this.deps).forEach(key => {
            if (key.includes('__')) return
            this.triggerUpdate(key, this.rue[key])
        })
        if(this.$options.onMounted) {
            this.$options.onMounted.apply(this)
        }
    }

    elementLoad(el) {
        const loaded = el.getAttribute('r_loaded')
        if (loaded === '1') return
        const val = el.getAttribute('r-load')
        if (val === '') return
        const event_handler = this.$options.methods[val]
        event_handler.apply(this, [el])
        el.setAttribute('r_loaded', '1')
    }
}

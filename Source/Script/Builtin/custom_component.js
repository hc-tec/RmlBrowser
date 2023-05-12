

class CustomComponentMap {
    constructor() {
        this.map = {}
    }
    register(name, assets) {
        this.map[name] = assets
    }
    exist(name) {
        return this.map[name] !== undefined
    }
    getAssets(name) {
        return this.map[name]
    }
}

const customComponentMap = new CustomComponentMap();

class CustomComponent {
    constructor(name) {
        this.name = name
        this.rml = ''
        this.rcss = ''
        this.script = ''
    }
    setRml(rml) {
        this.rml = rml
        return this
    }
    setRcss(rcss) {
        this.rcss = rcss
        return this
    }
    setScript(script) {
        this.script = script
        return this
    }
    register() {
        const custom = new dom.CustomComponent(this.name)
        custom
            .setRml(this.rml)
            .setRcss(this.rcss)
            .setScript(this.script)
            .register();
        customComponentMap.register(this.name, {
            rml: this.rml,
            rcss: this.rcss,
            script: this.script,
        })
    }
}

const slotDataMap = {}


function parseCustomComponent() {
    // const root = document.getElementsByTagName('body')[0]
    // log(`root ${root}`)
    while (CCustomNeedParse()) {
        const el = CCustomGetParseEl()
        parse(el)
    }
}

const parse = (root) => {
    const tag = root.getTagName()
    if (customComponentMap.exist(tag)) {
        parseSlotData(root)
        const id = root.getAttribute('r_id')
        const rml = customComponentMap.getAssets(tag).rml.replace('__rue__', id)
        root.innerRML = rml
        return
    }
    const child_num = root.getNumChildren(false)
    for (let i = 0; i < child_num; ++i) {
        const child = root.getChild(i)
        parse(child)
    }
}

const parseSlotData = (root) => {
    const child_num = root.getNumChildren(false)
    const id = root.getAttribute('r_id')
    log(root.getTagName())
    for (let i = 0; i < child_num; ++i) {
        const child = root.getChild(i)
        const tag = child.getTagName()
        log(tag)
        if (tag === 'template') {
            const slot = child.getAttribute('slot')
            if (slot) {
                slotDataMap[`${id}__${slot}`]  = child.innerRML
                CCustomSlotSet(`${id}__${slot}`, child.innerRML)
            } else {
                CCustomSlotSet(`${id}__default`, child.innerRML)
                slotDataMap[`${id}__default`]  = child.innerRML
            }
        }
    }
    if (!slotDataMap[`${id}__default`]) {
        CCustomSlotSet(`${id}__default`, root.innerRML)
        slotDataMap[`${id}__default`] = root.innerRML
    }
}
















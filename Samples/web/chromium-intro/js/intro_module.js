

class IntroModule {

    constructor(parent_ele) {
        this.parent_ele = parent_ele
        this.modules = []
    }

    Append(bg_img, title, description) {
        this.modules.push({
            bg_img,
            title,
            description
        })
        return this
    }

    BuildElement(module) {
        const intro_module = document.createElement(document, 'div')
        intro_module.setClassNames('intro-module')
        intro_module.setAttribute('style', `decorator: image(${module.bg_img} cover);`)
        const intro_module_container = document.createElement(document, 'div')
        intro_module_container.setClassNames('intro-module-container')
        const intro_title = document.createElement(document, 'p')
        intro_title.setClassNames('title')
        intro_title.innerRML = module.title
        const intro_description = document.createElement(document, 'p')
        intro_description.setClassNames('description')
        intro_description.innerRML = module.description
        intro_module_container.appendChild(intro_module_container, intro_title)
        intro_module_container.appendChild(intro_module_container, intro_description)
        intro_module.appendChild(intro_module, intro_module_container)
        this.parent_ele.appendChild(this.parent_ele, intro_module)
    }

    Build() {
        this.modules.forEach(module => {
            this.BuildElement(module)
        })
    }

}




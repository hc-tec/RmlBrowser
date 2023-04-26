

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
        const intro_module = document.createElement('div')
        intro_module.setClassNames('intro-module')
        intro_module.setAttribute('style', `decorator: image(${module.bg_img} cover);`)
        const intro_module_container = document.createElement('div')
        intro_module_container.setClassNames('intro-module-container')
        const intro_title = document.createElement('p')
        intro_title.setClassNames('title')
        intro_title.innerRML = module.title
        const intro_description = document.createElement('p')
        intro_description.setClassNames('description')
        intro_description.innerRML = module.description
        this.parent_ele.appendChild(intro_module, true)
        intro_module_container.appendChild(intro_title, true)
        intro_module_container.appendChild(intro_description, true)
        intro_module.appendChild(intro_module_container, true)

    }

    Build() {
        this.modules.forEach(module => {
            this.BuildElement(module)
        })
    }

}


// const intro_module_wrapper = document.getElementsByClassName('intro-module-wrapper')[0]
// const intro_module = new IntroModule(intro_module_wrapper)
// intro_module.Append('./images/rI6G204.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
//     .Append('./images/ss3c252_ANI-4.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
//     .Append('./images/WAN2698_ANI-1-2.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
//     .Append('./images/Snipaste_2023-01-04_20-48-41_prev_ui.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
//     .Append('./images/ndRm753_1212.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
//     .Build()


class Subject {
    constructor() {
        this.observers = []
    }
    attach(observer) {
        this.observers.push(observer)
    }
    detach(observer) {
        let index = -1
        this.observers.some((obs,i) => {
            if (obs === observer) {
                index = i
                return true
            }
            return false
        })
        if (index !== -1) {
            this.observers.splice(index, 1)
        }
    }
}

class TabManager extends Subject {

    constructor(tab_parent_el) {
        super()
        this.tab_parent_el = tab_parent_el
        this.tabs = {}
        this.tab_num = 0
        this.active_tab = null
    }

    get_active_tab_params() { return this.tabs[this.active_tab] }

    add_tab(params) {
        const id = params.id
        if (this.tabs[id]) return
        this.tab_num++
        this.tabs[id] = params
        const tab = document.createElement('div')
        tab.setClassNames('tab')
        tab.setId(id)
        if (this.tab_num === 1) {
            tab.setClassNames('tab tab-active')
            this.active_tab = id
        }
        tab.addEventListener(tab, 'click', e => {
            const el = e.getCurrentElement()
            const tab_id = el.getId()
            CFocusTab(tab_id)
            // this.on_tab_active(tab_id)
        })
        const title = document.createElement('p')
        title.innerRML = params.title
        const close_icon = document.createElement('p')
        close_icon.setClassNames('close-icon')
        close_icon.innerRML = 'x'
        close_icon.addEventListener(close_icon, 'click', e => {
            e.stopPropagation()
            const el = e.getTargetElement().getParentNode()
            const tab_id = el.getId()
            this.remove_tab(id)
            const focus_id = Object.keys(this.tabs)[this.tab_num-1]
            CRemoveTab(tab_id, focus_id)
        })
        tab.appendChild(title, true)
        tab.appendChild(close_icon, true)

        const add_icon = document.getElementsByClassName('add-icon')[0]
        this.tab_parent_el.insertChild(tab, add_icon)
        this.NotifyTabAdd(tab)
        log(`Id: ${id} title: ${params.title} added to Tabs`)
    }

    remove_tab(id) {
        if (!this.tabs[id]) return
        log(`remove_tab ${id}`)
        delete this.tabs[id]
        this.tab_num--
        const tab = document.getElementById(id)
        this.NotifyTabRemove(tab)
        this.tab_parent_el.removeChild(tab)
    }

    on_tab_active(id) {
        if (!this.tabs[id]) return
        // if (id === this.active_tab) return
        log(`on_tab_active ${this.active_tab} -> ${id}`)
        this.unfocus_tab(this.active_tab)
        this.focus_tab(id)
    }

    on_tab_fresh(params) {
        // const old_params = this.tabs[params.id]
        this.tabs[params.id] = params
    }

    unfocus_tab(id) {
        if (!this.tabs[id]) return
        const tabs = this.tab_parent_el.getElementsByClassName('tab')
        for (let i = 0; i < this.tab_num; i++) {
            const tab = tabs[i]
            if (tab.getId() === id) {
                tab.setClassNames('tab')
                break
            }
        }
    }

    focus_tab(id) {
        this.active_tab = id
        const tabs = this.tab_parent_el.getElementsByClassName('tab')
        for (let i = 0; i < this.tab_num; i++) {
            const tab = tabs[i]
            if (tab.getId() === id) {
                tab.setClassNames('tab tab-active')
                this.NotifyTabFocus(tab)
                break
            }
        }
    }

    NotifyTabAdd(tab) {
        const params = this.tabs[tab.getId()]
        this.observers.forEach(observer => {
            observer.onTabAdd(tab, params)
        })
    }

    NotifyTabRemove(tab) {
        const params = this.tabs[tab.getId()]
        this.observers.forEach(observer => {
            observer.onTabRemove(tab, params)
        })
    }

    NotifyTabFocus(tab) {
        const params = this.tabs[tab.getId()]
        this.observers.forEach(observer => {
            observer.onTabFocus(tab, params)
        })
    }

}

class TabManagerObserver {
    onTabAdd(tab, params) {}
    onTabRemove(tab, params) {}
    onTabFocus(tab, params) {}
}

const tab_parent_el = document.getElementsByClassName('tabs-container')[0]
const tab_manager = new TabManager(tab_parent_el)

const add_icon = document.getElementsByClassName('add-icon')[0]
add_icon.addEventListener(add_icon, 'click', e => {
    COpenTabWithUrl('-1', '')
})

function TAB_MANAGER_ADD_TAB(params) {
    tab_manager.add_tab(params)
}

function TAB_MANAGER_REMOVE_TAB(id) {
    tab_manager.remove_tab(id)
}

function TAB_MANAGER_ON_TAB_ACTIVE(id) {
    tab_manager.on_tab_active(id)
}

function TAB_MANAGER_FRESH_TAB(params) {
    tab_manager.on_tab_fresh(params)
}


/* Search Input */
let search_input = document.getElementById('search-input')
search_input = dom.ToElementFormControl(search_input)

// observe tab focus
class SearchInputObserver extends TabManagerObserver {
    onTabFocus(tab, params) {
        log('SearchInputObserver')
        search_input.value = params.url
    }
}
const search_input_observer = new SearchInputObserver()
tab_manager.attach(search_input_observer)

// listen keydown event
search_input.addEventListener(search_input, 'keydown', e => {
    const params = e.getParameters()
    const key_identifier = params['key_identifier']
    // press enter
    if (key_identifier === 72) {
        const input_value = search_input.value
        log(`input_value ${input_value}`)
        const params = tab_manager.get_active_tab_params()
        if (params.url === input_value) return
        COpenTabWithUrl(params.id, input_value)
    }
})






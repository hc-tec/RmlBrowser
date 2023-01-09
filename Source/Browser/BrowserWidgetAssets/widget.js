

class TabManager {

    constructor(tab_parent_el) {
        this.tab_parent_el = tab_parent_el
        this.tabs = {}
    }

    add_tab(id, title_) {
        if (this.tabs[id]) return
        this.tabs[id] = 1
        const tab = document.createElement(document, 'div')
        tab.setClassNames('tab')
        tab.setId(id)
        if (Object.keys(this.tabs).length === 1) {
            tab.setClassNames('tab tab-active')
        }
        tab.addEventListener(tab, 'click', e => {
            const el = e.getCurrentElement()
            const tab_id = el.getId()
            CFocusTab(tab_id)
        })
        const title = document.createElement(document, 'p')
        title.innerRML = title_
        const close_icon = document.createElement(document, 'p')
        close_icon.setClassNames('close-icon')
        close_icon.innerRML = 'x'
        close_icon.addEventListener(close_icon, 'click', e => {
            const el = e.getCurrentElement()
            const tab_id = el.getId()
            CRemoveTab(tab_id)
        })
        tab.appendChild(tab, title)
        tab.appendChild(tab, close_icon)
        this.tab_parent_el.appendChild(this.tab_parent_el, tab)
        log(`Id: ${id} title: ${title_} added to Tabs`)
    }

    remove_tab(id) {
        delete this.tabs[id]
        const tab = document.getElementById(id)
        tab_parent_el.removeChild(tab_parent_el, tab)
        log('remove tab')
    }

}
const tab_parent_el = document.getElementsByClassName('tabs')[0]
const tab_manager = new TabManager(tab_parent_el)

function TAB_MANAGER_ADD_TAB(id, title) {
    tab_manager.add_tab(id, title)
}









// import './intro_module.js'

// press F5, fresh
document.addEventListener(document, 'keydown', (e) => {
    const params = e.getParameters()
    const key_identifier = params['key_identifier']
    // F5 = 111
    if (key_identifier === 111) {
        reload()
    }
})









document.addEventListener(document, 'keydown', (e) => {
    const params = e.getParameters()
    const key_identifier = params['key_identifier']
    // press F5, fresh
    if (key_identifier === 111) {
        reload()
    }
})




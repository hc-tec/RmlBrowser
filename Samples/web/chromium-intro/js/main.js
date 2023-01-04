
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

const intro_module_wrapper = document.getElementsByClassName('intro-module-wrapper')[0]
const intro_module = new IntroModule(intro_module_wrapper)
intro_module.Append('./images/rI6G204.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
    .Append('./images/ss3c252_ANI-4.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
    .Append('./images/WAN2698_ANI-1-2.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
    .Append('./images/Snipaste_2023-01-04_20-48-41_prev_ui.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
    .Append('./images/ndRm753_1212.png', 'Thread', 'The network stack is a mostly single-threaded cross-platform library primarily for resource fetching.')
    .Build()








// press F5, fresh
document.addEventListener(document, 'keydown', (e) => {
    const params = e.getParameters()
    const key_identifier = params['key_identifier']
    // F5 = 111
    if (key_identifier === 111) {
        reload()
    }
    if (key_identifier === 107) {
        window_exit()
    }
})


class History {
    constructor() {
        this.his = []
        this.el = document.getElementsByClassName('historys')[0]
    }

    set_history(his) {
        this.his = his
    }

    render() {
        this.his.forEach(el => {
            const his = this.render_one(el)
            this.el.appendChild(his, true)
        })
    }

    render_one(params) {
        const star = document.createElement('div')
        star.setClassNames('history')
        const icon = document.createElement('img')
        icon.setClassNames('icon')
        icon.setAttribute('src', params.icon)
        star.appendChild(icon, true)
        let p = document.createElement('p')
        p.setClassNames('title')
        p.innerRML = params.title
        star.appendChild(p, true)

        p = document.createElement('p')
        p.setClassNames('url')
        p.innerRML = params.url
        star.appendChild(p, true)

        p = document.createElement('p')
        p.setClassNames('time')
        p.innerRML = params.time
        star.appendChild(p, true)

        star.setAttribute('url', params.url)
        star.addEventListener(star, 'click', e => {
            const url = star.getAttribute('url')
            COpenTabWithUrl("-1", url)
        })

        return star
    }

    push(params) {
        this.his.push(params)
        this.el.appendChild(this.render_one(params), true)
    }

}

const his = new History()

his.set_history(CHistoryGet().reverse())
his.render()



// const axios = new Axios()
// axios.defaults.baseURL = 'http://148.70.204.53:3001'
// axios.interceptors.request.use(
//     config => {
//         log('intercept request')
//         config.headers['Authorization'] = 'JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyX2lkIjo3MjIsInVzZXJuYW1lIjoiMTUyNzA5NDk0NjYiLCJleHAiOjE2ODIyMTM3ODcsImVtYWlsIjoiIn0.MZ8OorW1_aYaLd2nV3AqFm8220Or1T9XVxqtHh8UuD8'
//         return config
//     },
//     config => {
//         return config
//     }
// )
//
// const res = axios.get("/api/member/mbr/", {
//     username: 'test3',
//     password: 'sun19961203'
// })
// log(JSON.stringify(res))
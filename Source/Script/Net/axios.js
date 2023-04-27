

function InterceptorManager() {
    this.handlers = [];
}

InterceptorManager.prototype.use = function use(fulfilled, rejected) {
    this.handlers.push({
        fulfilled: fulfilled,
        rejected: rejected
    });
    // 返回当前的索引，用于移除已注册的拦截器
    return this.handlers.length - 1;
};

function Axios(instanceConfig) {
    this.defaults = instanceConfig || {};
    this.http = new net.Http()
    this.beforeRes = {}
    this.interceptors = {
        request: new InterceptorManager(),
        response: new InterceptorManager()
    };
}

const dispatchRequest = {
    fulfilled(config) {
        if (config.baseURL && config.baseURL.length && config.url[0] === '/') {
            config.url = config.baseURL + config.url
        }
        switch (config.method) {
            case 'get':
                const keys = Object.keys(config.params)
                if (keys.length > 0) {
                    config.url += '?'
                    keys.forEach(key => {
                        config.url += `${key}=${config.params[key]}&`
                    })
                    config.url = config.url.slice(0, config.url.length - 1)
                }
                this.http.get(config.url, {}, config.headers, {
                    axios_cb: this.callback,
                    _this: this,
                    user_cb: config.callback
                })
                break
            case 'post':
                this.http.post(config.url, config.params, config.headers, {
                    axios_cb: this.callback,
                    _this: this,
                    user_cb: config.callback
                })
                break
        }
        log('send over')
        return false
    },
    rejected(config) {
        return config
    }
}

Axios.prototype.request = function request(config) {
    if (config.headers === undefined) config.headers = {}
    if (config.params === undefined) config.params = {}
    config = Object.assign(config, this.defaults);
    // ...
    var chain = [dispatchRequest];
    // 任务编排
    this.interceptors.request.handlers.forEach(function unshiftRequestInterceptors(interceptor) {
        chain.unshift(interceptor);
    });
    this.interceptors.response.handlers.forEach(function pushResponseInterceptors(interceptor) {
        chain.push(interceptor);
    });
    // 任务调度
    let i = 0;
    while (i < chain.length) {
        if (chain[i] === null) continue
        try {
            config = chain[i].fulfilled.apply(this, [config])
            if (!config) {
                log(`config ${config}`)
                i++;
                break;
            }
        } catch (e) {
            log(e)
            chain[i].rejected(config)
            break
        }
        i++;
    }
    this.beforeRes.chain = []
    if (i < chain.length) {
        this.beforeRes.chain = chain.slice(i)
    }
}

Axios.prototype.callback = function (config) {
    log(`call callback ${config}`)
    const chain = this.beforeRes.chain
    // 任务调度
    log(`chain ${chain.length}`)
    let i = 0;
    while (i < chain.length) {
        if (chain[i] === null) continue
        try {
            config = chain[i].fulfilled(config)
        } catch (e) {
            log(e)
            chain[i].rejected(config)
            break
        }
        i++;
    }
    config._u_cb(config)
}

Axios.prototype.get = function (url, params, callback, headers) {
    return this.request({
        method: 'get',
        url,
        params,
        headers,
        callback,
    })
}

Axios.prototype.post = function (url, params, callback, headers) {
    return this.request({
        method: 'post',
        url,
        params,
        headers,
        callback,
    })
}








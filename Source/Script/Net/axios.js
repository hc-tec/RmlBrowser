

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
    this.interceptors = {
        request: new InterceptorManager(),
        response: new InterceptorManager()
    };
}

const dispatchRequest = {
    fulfilled: config => {
        const req = new net.Http()
        if (config.baseURL && config.baseURL.length && config.url[0] === '/') {
            config.url = config.baseURL + config.url
        }
        let res
        switch (config.method) {
            case 'get':
                const keys = Object.keys(config.params)
                if (keys.length > 0) config.url += '?'
                keys.forEach(key => {
                    config.url += `${key}=${config.params[key]}&`
                })
                config.url = config.url.slice(0, config.url.length-1)
                res = req.get(config.url, {}, config.headers)
                break
            case 'post':
                res = req.post(config.url, config.params, config.headers)
                break
        }
        try {
            res.data = JSON.parse(res.data)
        } catch (e) {
        }
        return res
    },
    rejected: config => {
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
            config = chain[i].fulfilled(config)
        } catch (e) {
            log(e)
            chain[i].rejected(config)
            break
        }
        i++;
    }
    return config
}

Axios.prototype.get = function (url, params, headers) {
    return this.request({
        method: 'get',
        url,
        params,
        headers,
    })
}

Axios.prototype.post = function (url, params, headers) {
    return this.request({
        method: 'post',
        url,
        params,
        headers,
    })
}








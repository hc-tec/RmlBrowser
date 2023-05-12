

rml = `
<div 
    id="__rue__">
    <div r-if="networkRequestsVisible">
        <table>
            <thead>
                <tr>
                    <td colspan="4">名称</td>
                    <td>方法</td>
                    <td>状态</td>
                    <td>协议</td>
                    <td>大小</td>
                </tr>
            </thead>
            <tbody>
                <tr
                        r-for="req in networkRequests"
                        r-netid="req.header.net-id"
                        r-click="handleNetworkItemClick">
                    <td colspan="4">{{ req.url }}</td>
                    <td>{{ req.method }}</td>
                    <td>200</td>
                    <td>h1</td>
                    <td>111</td>
                </tr>
            </tbody>
        </table>
    </div>
    <div
            id="network-details"
            r-if="networkDetailsVisible">
        <tabset
                id="network-details-tabset"
                style="position: relative">
            <tab style="width: 14px;padding: 0;">
                <img src="static/imgs/close.png" r-click="closeNetworkDetails" />
            </tab>
            <panel></panel>
            <tab>标头</tab>
            <panel>
                <div class="network-details-head">
                    <p>常规</p>
                    <div>
                        <div>
                            <span>请求URL：</span>
                            <span>{{ chosenNetworkItem.request.url }}</span>
                        </div>
                        <div>
                            <span>请求方法：</span>
                            <span>{{ chosenNetworkItem.request.method }}</span>
                        </div>
                        <div>
                            <span>状态代码：</span>
                            <span>{{ chosenNetworkItem.response.status }}</span>
                        </div>
                        <div>
                            <span>远程地址：</span>
                            <span>{{ chosenNetworkItem.request.header.Host }}</span>
                        </div>
                    </div>
                </div>
                <div class="network-details-head">
                    <p>响应头</p>
                    <div>
                        <div
                                r-for="response in networkDetailsResponseHeaders">
                            <span>{{ response.key }}：</span>
                            <span>{{ response.value }}</span>
                        </div>
                    </div>
                </div>
                <div class="network-details-head">
                    <p>请求头</p>
                    <div>
                        <div
                            r-for="request in networkDetailsRequestHeaders">
                            <span>{{ request.key }}：</span>
                            <span>{{ request.value }}</span>
                        </div>
                    </div>
                </div>
            </panel>
            <tab>负载</tab>
            <panel>
                {{ chosenNetworkItem.request.body }}
            </panel>
            <tab>响应</tab>
            <panel>
                <div class="whitespace-pre-wrap break-words">
                    {{ chosenNetworkItem.response.body }}
                </div>
            </panel>
        </tabset>
    </div>
</div>
`
script = `
<script>
const __rue__ = new Rue({
    el: '__rue__',
    data: {
        networkRequestsMap: {},
        networkRequests: [],
        chosenNetworkItem: {},
        networkRequestsVisible: false,
        networkDetailsVisible: false,
        networkDetailsRequestHeaders: [],
        networkDetailsResponseHeaders: [],
    },
    methods: {
        handleNetworkItemClick(e) {
            const net_id = e.getAttribute('netid')
            this.chosenNetworkItem = this.networkRequestsMap[net_id]
            this.chosenNetworkItem.response.body = this.chosenNetworkItem.response.body.replace(/^\\s+|\\s+$/g, '')
            this.networkDetailsRequestHeaders = []
            Object.keys(this.chosenNetworkItem.request.header).forEach(key => {
                this.networkDetailsRequestHeaders.push({key, value: this.chosenNetworkItem.request.header[key]})
            })
            this.networkDetailsRequestHeaders = this.networkDetailsRequestHeaders
            this.networkDetailsResponseHeaders = []
            Object.keys(this.chosenNetworkItem.response.header).forEach(key => {
                this.networkDetailsResponseHeaders.push({key, value: this.chosenNetworkItem.response.header[key]})
            })
            this.networkDetailsResponseHeaders = this.networkDetailsResponseHeaders
            this.networkDetailsVisible = true
            this.initNetworkDetailTabset()
        },
        closeNetworkDetails() {
            this.networkDetailsVisible = false
        },
        
    },
    
})
rml.net.observeNetwork()
rml.net.onRequestStart = (e) => {
    try {
        const net_id = e.header['net-id']
        __rue__.networkRequests.push(e)
        __rue__.networkRequests = __rue__.networkRequests
        __rue__.networkRequestsVisible = true
        log(JSON.stringify(e))
    } catch (err) {
        log('errxx ' + err)
    }
}
rml.net.onRequestEnd = (e) => {
    const net_id = e.request.header['net-id']
    __rue__.networkRequestsMap[net_id] = e
}
`.slice(9)

style = `
<style>

`.slice(8)

component = new CustomComponent('network')
component
    .setRml(rml)
    .setRcss(style)
    .setScript(script)
    .register();










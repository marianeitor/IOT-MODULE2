
function getData() {
    fetch('/getData').then(function (response) {
        response.json().then(function (json) {
            console.log(json);
            var el = document.forms[0];
            el['essid'].value = json.essid;
            el['pass'].value = json.pass;
            el['mqttServer'].value = json.brokerAdd;
            el['mqttUser'].value = json.brokerUser;
            el['mqttPass'].value = json.brokerPass;
            el['mqttTopic'].value = json.brokerTopic;
            el['mqttPort'].value = json.brokerPort;
            el['clientId'].value = json.clientId;
        });
    });

}


getData();
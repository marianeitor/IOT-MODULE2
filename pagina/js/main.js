
function getData() {
    fetch('/getData').then(function (response) {
        response.json().then(function (json) {
            console.log(json);
            var el = document.forms[0];
            el['essid'].value = json.essid;
            el['pass'].value = json.pass;
            el = document.forms[1];
            el['mqttServer'].value = json.brokerAdd;
            el['mqttUser'].value = json.brokerUser;
            el['mqttPass'].value = json.brokerPass;
            el['mqttTopic'].value = json.brokerTopic;
            el['mqttPort'].value = json.brokerPort;
            el['clientId'].value = json.clientId;
        });
    });

}

function getMqttStatus() {
    fetch('/getMqttStatus').then(function (response) {
        response.json().then(function (json) {
            if (json.CONNECTED) {
                document.getElementById("mqttStatus").style.color = "#5cb85c";
                document.getElementById("mqttStatus").innerText = "Conectado";
            }
            else {
                document.getElementById("mqttStatus").style.color = "#d9534f";
                document.getElementById("mqttStatus").innerText = "Desconectado";
            }
        });
    });
}

function buscarRedes() {
    document.getElementById("networkLoader").style.display = "";
    fetch('/getNetworks').then(function (response) {
        response.json().then(function (json) {
            console.log(json);

            document.getElementById("networksList").style.display = "";
            document.getElementById("networkLoader").style.display = "none";

            var tbody = document.getElementById("networksListBody")
            tbody.innerHTML = "";
            for (var index in json) {

                var row = tbody.insertRow();
                row.setAttribute("onclick", "setSSID('" + json[index].ESSID + "')");
                var cellSSID = row.insertCell();
                var cellSeguridad = row.insertCell();
                var cellSignal = row.insertCell();

                cellSSID.innerHTML = json[index].ESSID;
                cellSeguridad.innerHTML = (json[index].ENCRYPTION == 1) ? "SI" : "NO";
                cellSignal.innerHTML = json[index].SIGNAL_LEVEL + "%";
            }
        });
    });
}

function setSSID(ssid) {
    document.getElementById("networksList").style.display = "none";

    var el = document.forms[0];
    el['essid'].value = ssid;
    el['pass'].value = "";

}

function togglePassVisible() {
    var passwordFiel = document.getElementById('pass');
    if (passwordFiel.type == 'password') {
        passwordFiel.type = 'text';
    }
    else {
        passwordFiel.type = 'password';
    }
}

getData();
getMqttStatus();
setInterval(function() {
    getMqttStatus();
}, 5000);
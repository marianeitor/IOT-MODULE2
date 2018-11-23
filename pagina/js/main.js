
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

            var tbody = document.getElementById("networksListBody");

            json = json.sort(function (a,b) {
                if (a.SIGNAL_LEVEL < b.SIGNAL_LEVEL)
                    return 1;
                if (a.SIGNAL_LEVEL > b.SIGNAL_LEVEL)
                    return -1;
                return 0;
            });

            tbody.innerHTML = "";
            for (var index in json) {

                var row = tbody.insertRow();
                row.setAttribute("onclick", "setSSID('" + json[index].ESSID + "', " + json[index].ENCRYPTION + ")");
                var cellSSID = row.insertCell();
                var cellSeguridad = row.insertCell();
                var cellSignal = row.insertCell();

                var imgLock = "<img  width=\"24\" height=\"24\"  src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAACuUlEQVR42mNkoDFgJKTAydVNOiQsIldNXcOHkZFR5f///yDhO7dv3dyybvXKybt37nhKtgVNbZ0xVja204GG8nz5/o3hx+9fIOH/HKxsjDycXP+ZmJi+njh+NLO6rGQJyRY0tXUADbdbCDSY8cW7Nwx//v5lAPoALAfyBQszM4OEkAgDyKKTJ47HV5cVLyHaAgcnF+naxuYbQMN5nrx++f/f3783j+zdW3Hh9Om9IMP1TU2cbZ1dOphZWDRkRMUZuDk4v3S2NmlgCy6sFkyeMbtDU0u7/P7zJww/fv68Ob2n2+LapUsfkNWoa2sLZJdXHOdkZ9dQlJT5f+fWra6MlIQKoizYeeDIlY9fPmu9+vCO4cjePcEr5s9fj01daGycv727+3oxASEGAV6+a2721jpEWTB16bIfQIodFNxTOtr5r1++/BmbOlVNTZ6CmtpPoGADxs+v7OgoDoIWmBnLMMYVdv4BamACcv+tm1XNsvfg/f/YLHC0kWUMyez8A7QAqJzx/5KJlSzHTz38j9OCmTWc/uHubDOXPJwmBpOMls8G2cOAHTAxLH04leE/XG3Wq3V7f6UnNXzfiNWCj0f4XwB9K771dsj/u+/VGJUFbzF4q64BOg+h7j/MueAcx8gIVMsAVPsfqJYRqBak4qWA7ScJrBZ8OMyH5FSQFNaQwQJQ1QItYMLug+38/xlfAOX+MIBdiexibD5AV/Of+T/Df4l///k9P2K34NN8wf+MfxmBGuDBygB1GrI6GB9DDcg6oCX/+RLfY7fg8xwhcMh+ecxPlgU8sh/B/uFLxWHBp9mC/4goYAmA/ww4LaC5D0AWQCKNgjj4T0QQAX1AVuBAfcAwcD6gUyqiHODzwQsgJY4tl8KdjycnQ9kvgRZgL4umxHP7x9uyzwQqEiPTgpfLj/9MT5v7FV6aAgBW56EoRlymCQAAAABJRU5ErkJggg==\">";

                cellSSID.innerHTML = json[index].ESSID;
                cellSeguridad.innerHTML = (json[index].ENCRYPTION == 1) ? imgLock : "<b>ABIERTA</b>";
                cellSignal.innerHTML = json[index].SIGNAL_LEVEL + "%";
            }
        });
    });
}

function setSSID(ssid, enc) {
    document.getElementById("networksList").style.display = "none";

    var el = document.forms[0];
    el['essid'].value = ssid;
    if (enc != 1) {
        el['pass'].disabled = "true";
    }
    else {
        el['pass'].disabled = "";
    }
    el['pass'].value = "";

}

function enablePassField() {
    var el = document.forms[0];
    el['pass'].disabled = "";
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
const client = require("./service/fileSrv/client");

client.Ping({id: 24}, (err, data) => {
    if (!err) {
        console.log(data);
    } else {
        console.log(err)
    }
});
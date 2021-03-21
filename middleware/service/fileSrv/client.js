const PROTO_PATH = "./protos/fileSrv.proto";

const grpc = require("grpc");
const protoLoader = require("@grpc/proto-loader");

var packageDefinition = protoLoader.loadSync(PROTO_PATH, {
    keepCase: true,
    longs: String,
    enums: String,
    arrays: true
});

const FileSrv = grpc.loadPackageDefinition(packageDefinition).daemon.FileSrv;
const client = new FileSrv(
    "localhost:30043",
    grpc.credentials.createInsecure()
);

module.exports = client;

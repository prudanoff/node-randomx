var rx = require('./build/Release/addon');

let key = "0410591dc8b3bba89f949212982f05deeb4a1947e939c62679dfc7610c62";
let blob = "58249adafb690683a800ee8d6556e2a7d25864d577afbf709ceff9e3bdd5ebae";

let vm;
console.log("Starting RandomX virtual machine...");
try{
	vm = rx.RandomxVM(key, ["jit", "ssse3"]);
}
catch(e){
	console.log(e);
	return 0;
}

console.log("Start hashing...");
let	hash = rx.hash(vm, blob);
console.log("hash: " + hash);
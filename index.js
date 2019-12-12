var rx = require('./build/Release/addon');
let crypto = require('crypto');

function difficulty(h){
	let count = 0;
	let i = 0;
	let bit = 0;
	do {
		bit = ((h[i/8|0]) & (1 << (7 - (i%8)))) >> (7 - (i%8));
		count += !(bit);
		i++;
	} while (!bit);
	return count;
}

let key = "0410591dc8b3bba89f949212982f05deeb4a1947e939c62679dfc7610c62";

let vm;
console.log("Starting RandomX virtual machine...");
try{
	vm = rx.RandomxVM(key);
}
catch(e){
	console.log(e);
	return 0;
}
let hash;
let total = 0;
let target = 4;

console.log("Start hashing...");
for(let i = 0; i < 100; i++){
	let block = {
		"foo" : (crypto.randomBytes(32)).toString('hex'),
		"bar" : (crypto.randomBytes(32)).toString('hex'),
		"nonce" : 0
	}
	do {
		let str = ['foo','bar','nonce'].map(v => block[v].toString()).join("");
		let blob = crypto.createHmac('sha256', '').update(str).digest().toString('hex');
		hash = rx.hash(vm, blob);
		block.nonce++;
	}
	while(!(difficulty(Buffer.from(hash, "hex")) >= target))
	console.log("Hash: " + hash + " Nonce: " + (block.nonce - 1));
	total += (block.nonce - 1)
}
console.log("Average nonce: " + (total / 100))
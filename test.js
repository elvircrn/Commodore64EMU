const fs = require('fs');
const buff = fs.readFileSync('buff.txt').toString();


const instrs = [
    'dey',
    '*',
    'dey',
    'dey',
    'dey'
];
const instrs1 = [
    'sta',
    'sta',
    'sta',
    'sta',
    'lda',
    'sta'
];


const extractInstruction = row =>
    row.trim().split(' ')[0];

let it = 0;
const rows = buff.split('\n');


for (let i = 0; i < rows.length; i++) {
    let j = 0;
    for (; j < instrs.length; j++) {
        const instr = extractInstruction(rows[i + j]);
        if (instrs[j] !== '*' && instr !== instrs[j]) {
            break;
        }
    }

    if (j === instrs.length) {
        for (let j = i - 2 * instrs.length; j < i + instrs.length; j++) {
            console.log(rows[j]);
        }
        console.log('\n\n');
    }
}


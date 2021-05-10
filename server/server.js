

const express = require('express');
const app = express();
const path = require('path')
app.set("view engine" , "ejs");
app.use(express.static(path.join(__dirname, "public")));
const port = 8080 || process.env.PORT;


const fs = require('fs');
const args = process.argv;
var out_json = JSON.parse(fs.readFileSync(`../build/${args[2]}`,'utf-8'));



var tree = {}

tree.chart = {
    container: "#chart",
    levelSeparation: 25,
    siblingSeparation: 70,
    subTeeSeparation: 70,
    nodeAlign: "BOTTOM",
    scrollbar: "fancy",
    padding: 35,
    node: { HTMLclass: "evolution-tree" },
    connectors: {
        type: "curve",
        style: {
            "stroke-width": 2,
            "stroke-linecap": "round",
            "stroke": "#ccc"
        }
    }
}

tree.nodeStructure = out_json;


app.get("/", (req,res) => {
    res.render("index",{
        tree : tree
    });
});

app.listen(port);
console.log('Server started on port' , port);
console.log('Go to http://localhost:8080 to view the Abstract Syntax Tree');



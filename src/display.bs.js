// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var React       = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");

var component = ReasonReact.statelessComponent("Display");

function make(value, _) {
  var newrecord = component.slice();
  newrecord[/* render */9] = (function () {
      return React.createElement("p", undefined, value);
    });
  return newrecord;
}

exports.component = component;
exports.make      = make;
/* component Not a pure module */

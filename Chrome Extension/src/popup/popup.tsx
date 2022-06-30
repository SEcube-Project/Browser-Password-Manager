import React, { useEffect } from "react";
import ReactDOM from "react-dom";
import App  from "./app";



// Root element for the popup
const root = document.createElement("div");
document.body.appendChild(root);
ReactDOM.render(<App />, root);


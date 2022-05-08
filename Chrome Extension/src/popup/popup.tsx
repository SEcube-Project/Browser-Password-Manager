import React, { useEffect } from "react";
import ReactDOM from "react-dom";
import { fetchApi } from "../utils/api";
import "./popup.css";
// variable to test the code without using real API but only JSON file
const DEBUG=true;

const App: React.FC<{}> = () => {
  return <div> Hello world from extension</div>;
};


const root = document.createElement("div");
document.body.appendChild(root);
ReactDOM.render(<App />, root);
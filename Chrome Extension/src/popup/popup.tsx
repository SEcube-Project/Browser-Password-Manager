import React, { useEffect } from "react";
import ReactDOM from "react-dom";
import { fetchApi } from "../utils/api";
import "./popup.css";

import Box from '@mui/material/Box';
import SpeedDial from '@mui/material/SpeedDial';
import SpeedDialIcon from '@mui/material/SpeedDialIcon';
import SpeedDialAction from '@mui/material/SpeedDialAction';
import FileCopyIcon from '@mui/icons-material/FileCopyOutlined';
import SaveIcon from '@mui/icons-material/Save';
import PrintIcon from '@mui/icons-material/Print';
import ShareIcon from '@mui/icons-material/Share';


// variable to test the code without using real API but only JSON file
const DEBUG=true;

const App: React.FC<{}> = () => {
  useEffect(() => {
    fetchApi(DEBUG)
      .then((res) => {
        console.log(res);
      })
      .catch((err) => {
        console.log(err);
      });
  }, []);
  return <div>hello world</div>
};


const root = document.createElement("div");
document.body.appendChild(root);
ReactDOM.render(<App />, root);
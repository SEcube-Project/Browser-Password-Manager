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
  return BasicSpeedDial();
};


const actions = [
  { icon: <FileCopyIcon />, name: 'Copy' },
  { icon: <SaveIcon />, name: 'Save' },
  { icon: <PrintIcon />, name: 'Print' },
  { icon: <ShareIcon />, name: 'Share' },
];


export default function BasicSpeedDial() {
  return (
    <Box sx={{ height: 500, width: 500, transform: 'translateZ(0px)', flexGrow: 1 }}>
      <SpeedDial
        ariaLabel="SpeedDial basic example"
        sx={{ position: 'absolute', bottom: 1, right: 1, top: 1, left: 420 }}
        icon={<SpeedDialIcon />}
      >
        {actions.map((action) => (
          <SpeedDialAction onClick={() => {
            console.log("clicked");
          }}
            key={action.name}
            icon={action.icon}
            tooltipTitle={action.name}
          />
        ))}
      </SpeedDial>
    </Box>
  );
}


const root = document.createElement("div");
document.body.appendChild(root);
ReactDOM.render(<App />, root);
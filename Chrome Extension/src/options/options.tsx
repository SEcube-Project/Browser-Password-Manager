import React from "react";
import ReactDOM from "react-dom";
import "./options.css";
import { Grid, Box } from "@mui/material";
import * as ui_component from "./ui_component";


const App: React.FC<{}> = () => {
  return (
    <div>
      <Box mx="5%" my="5%">
        <Grid container spacing={2} direction="column">
          <Grid item>{ui_component.OptionsPageHeader()}</Grid>
          <Grid item>{ui_component.SwitchLabels("Autocomplete")}</Grid>
          <Grid item>{ui_component.BasicButtons()}</Grid>
        </Grid>
      </Box>
    </div>
  );
};

const root = document.createElement("div");
document.body.appendChild(root);
ReactDOM.render(<App />, root);


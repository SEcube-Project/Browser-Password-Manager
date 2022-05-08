import React from "react";
import ReactDOM from "react-dom";
import "./options.css";
import {
  Grid,
  Box,
  TextField,
  InputAdornment,
  FormGroup,
  FormControlLabel,
  Switch,
  Stack,
  Button,
} from "@mui/material";
import * as ui_component from "./static_ui_component";
import {
  getStoredOptions,
  LocalStorageOptions,
  setStoredOptions,
} from "../utils/storage";

const App: React.FC<{}> = () => {
  const [options, setOptions] = React.useState<LocalStorageOptions | null>(
    null
  );

  React.useEffect(() => {
    getStoredOptions().then((option) => {
      setOptions(option);
    });
  }, []);

  // Handle function
  const handleLockValueChange = (value: string) => {
    console.log(value);

    // If value contains NaN, set to 0
    if (isNaN(Number(value))) {
      setOptions({ 
        ...options, 
        lock_after_minutes: 0 
      });
    } else {
      setOptions({ 
        ...options, 
        lock_after_minutes: Number(value) 
      });
    }
  };

  return (
    <div>
      <Box mx="5%" my="5%">
        <Grid container spacing={2} direction="column">
          <Grid item>{ui_component.OptionsPageHeader()}</Grid>
          <Grid item>
            {" "}
            <FormGroup>
              <FormControlLabel
                control={<Switch defaultChecked />}
                label="Autocomplete"
              />
            </FormGroup>
          </Grid>
          <Grid item>
            {" "}
            <TextField
              label="Lock After"
              id="outlined-start-adornment"
              sx={{ m: 1, width: "25ch" }}
              InputProps={{
                startAdornment: (
                  <InputAdornment position="start">min</InputAdornment>
                ),
              }}
              value={String(options?.lock_after_minutes)}
              onChange={(e) => {
                handleLockValueChange(e.target.value);
              }}
            />
          </Grid>
          <Grid item>
            {" "}
            <Stack spacing={2} direction="row">
              <Button variant="contained">Save</Button>
            </Stack>
          </Grid>
        </Grid>
      </Box>
    </div>
  );
};

const root = document.createElement("div");
document.body.appendChild(root);
ReactDOM.render(<App />, root);

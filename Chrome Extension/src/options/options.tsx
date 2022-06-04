import React from "react";
import ReactDOM from "react-dom";

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
  Snackbar,
  SnackbarOrigin,
} from "@mui/material";
import { SnackbarProvider, VariantType, useSnackbar } from "notistack";

import "./options.css";
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

  /**
   * Handler of the text field for the Lock After Timeout
   * @param value - the input string
   */
  const handleLockValueChange = (value: string) => {
    // console.log(value);

    // If value contains NaN, set to 0
    if (isNaN(Number(value))) {
      setOptions({
        ...options,
        lock_after_minutes: 0,
      });
    } else {
      setOptions({
        ...options,
        lock_after_minutes: Number(value),
      });
    }
  };

  /**
   * Function to manage the click of the save button
   * @returns
   */
  function MyApp() {
    const { enqueueSnackbar } = useSnackbar();

    const handleClickVariant = (variant: VariantType) => () => {
      // variant could be success, error, warning, info, or default
      setStoredOptions(options);
      enqueueSnackbar("Options saved correctly", { variant });
    };

    return (
      <React.Fragment>
        <Button variant="contained" onClick={handleClickVariant("success")}>
          Save
        </Button>
      </React.Fragment>
    );
  }

  /**
   * Function to manage the render of the success snackbar
   * @returns
   */
  function IntegrationNotistack() {
    return (
      <SnackbarProvider maxSnack={3}>
        <MyApp />
      </SnackbarProvider>
    );
  }

  function handleAutocompleteChange(event: React.ChangeEvent<HTMLInputElement>) {
    setOptions({
      ...options,
      is_autocomplete_enabled: event.target.checked,
    });
  }

  return (
    <div>
      <Box mx="5%" my="5%">
        <Grid container spacing={2} direction="column">
          <Grid item>{ui_component.OptionsPageHeader()}</Grid>
          <Grid item>
            <FormGroup>
              <FormControlLabel
                control={<Switch checked={options?.is_autocomplete_enabled ?? false} onChange={handleAutocompleteChange} />}
                label="Autocomplete"
              />
            </FormGroup>
          </Grid>
          <Grid item>
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
            <Stack spacing={2} direction="row">
              {IntegrationNotistack()}
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

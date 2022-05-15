import * as React from "react";
import FormGroup from "@mui/material/FormGroup";
import FormControlLabel from "@mui/material/FormControlLabel";
import Checkbox from "@mui/material/Checkbox";
import {
  Box,
  Button,
  FormControl,
  IconButton,
  InputAdornment,
  InputLabel,
  OutlinedInput,
} from "@mui/material";
import { useSnackbar, VariantType, SnackbarProvider } from "notistack";
import Visibility from "@mui/icons-material/Visibility";
import VisibilityOff from "@mui/icons-material/VisibilityOff";
import { useState } from "react";

export default function GeneratePasswordElement() {
  return <CheckboxLabels />;
}

function CheckboxLabels() {
  const [showPassword, setShowPassword] = useState(false);
  const [password, setPassword] = useState("");

  const handleClickShowPassword = () => {
    setShowPassword(!showPassword);
  };

  const handleMouseDownPassword = (
    event: React.MouseEvent<HTMLButtonElement>
  ) => {
    event.preventDefault();
  };

  function MyApp() {
    const handleClick = () => {
      setPassword("ciao come stai");
      // TODO: Send the parameters to generate the password to the backend
    };

    return (
      <React.Fragment>
        <Button variant="contained" onClick={handleClick}>
          Generate
        </Button>
      </React.Fragment>
    );
  }

  function IntegrationNotistack() {
    return (
      <SnackbarProvider maxSnack={3}>
        <MyApp />
      </SnackbarProvider>
    );
  }

  return (
    <Box
      sx={{ display: "flex", flexWrap: "wrap", maxWidth: 400, maxHeight: 400 }}
    >
      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <FormControlLabel control={<Checkbox defaultChecked />} label="A-Z" />
        <FormControlLabel control={<Checkbox defaultChecked />} label="0-9" />
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="-_.:;,?&%$!@#"
        />
      </FormControl>
      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <IntegrationNotistack />
      </FormControl>

      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <InputLabel htmlFor="outlined-adornment-password">Password</InputLabel>
        <OutlinedInput
          id="outlined-adornment-password"
          type={showPassword ? "text" : "password"}
          readOnly={true}
          value={password}
          endAdornment={
            <InputAdornment position="end">
              <IconButton
                aria-label="toggle password visibility"
                onClick={handleClickShowPassword}
                onMouseDown={handleMouseDownPassword}
                edge="end"
              >
                {showPassword ? <VisibilityOff /> : <Visibility />}
              </IconButton>
            </InputAdornment>
          }
          label="Password"
        />
      </FormControl>
    </Box>
  );
}

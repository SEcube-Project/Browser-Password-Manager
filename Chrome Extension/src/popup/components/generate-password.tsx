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
import { generatePassword } from "../../utils/api";

export default function GeneratePasswordElement() {
  return <CheckboxLabels />;
}

function CheckboxLabels() {
  const [showPassword, setShowPassword] = useState(false);
  const [upper, setUpper] = useState(true);
  const [special, setSpecial] = useState(true);
  const [numbers, setNumbers] = useState(true);
  const [length, setLength] = useState(8);
  const [password, setPassword] = useState("");

  const handleUpperChange = (event) => {
    setUpper(event.target.checked);
  };

  const handleSpecialChange = (event) => {
    setSpecial(event.target.checked);
  };

  const handleNumbersChange = (event) => {
    setNumbers(event.target.checked);
  };

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
      generatePassword(upper, special, numbers, length)
        .then((res) => {
          setPassword(res.generated);
        })
        .catch((err) => {
          console.log(err);
        });
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
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="A-Z"
          onChange={() => handleUpperChange(event)}
        />
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="0-9"
          onChange={() => handleNumbersChange(event)}
        />
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="-_.:;,?&%$!@#"
          onChange={() => handleSpecialChange(event)}
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

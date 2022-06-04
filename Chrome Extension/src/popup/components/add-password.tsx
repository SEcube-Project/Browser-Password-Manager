import Visibility from "@mui/icons-material/Visibility";
import VisibilityOff from "@mui/icons-material/VisibilityOff";
import {
  Box,
  FormControl,
  InputLabel,
  OutlinedInput,
  InputAdornment,
  IconButton,
  Button,
  Stack,
  Snackbar,
} from "@mui/material";
import { VariantType, useSnackbar, SnackbarProvider } from "notistack";
import React, { useEffect, useState } from "react";
import {
  getStoredOptions,
  LocalStorageOptions,
  setStoredOptions,
} from "../../utils/storage";

import { insertNewPassword } from "../../utils/api";

export default function AddPasswordElement(props) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [url, setUrl] = useState("");
  const [showPassword, setShowPassword] = useState(false);
  const [pin, setPin] = useState("");

    // track the changes on the props.pin with a use effect
    useEffect(() => {
      setPin(props.pin);
    }, [props.pin]);
    

  const handleClickShowPassword = () => {
    setShowPassword(!showPassword);
  };

  const handleMouseDownPassword = (
    event: React.MouseEvent<HTMLButtonElement>
  ) => {
    event.preventDefault();
  };

  const handlePasswordChange = (event) => {
    setPassword(event.target.value);
  };

  const handleUsernameChange = (event) => {
    setUsername(event.target.value);
  };

  const handleUrlChange = (event) => {
    setUrl(event.target.value);
  };

  React.useEffect(() => {
    setUrl(props.url);
  }, [props.url]);


  function MyApp() {
    const { enqueueSnackbar } = useSnackbar();
  
    const handleClickVariant = () => () => {
      // console.log("url", url, "username", username, "password", password);
      insertNewPassword(url, username, password, "POST")
      enqueueSnackbar("Password saved correctly", {variant: "success"});
    };
  
    return (
      <React.Fragment>
        <Button variant="contained" onClick={handleClickVariant()}>
          Save
        </Button>
      </React.Fragment>
    );
  }
  
  function IntegrationNotistack() {
    return (
      <SnackbarProvider maxSnack={1} autoHideDuration={1300}>
        <MyApp />
      </SnackbarProvider>
    );
  }

  return (
    <Box
      sx={{
        display: "flex",
        flexWrap: "wrap",
        maxWidth: "100%",
        maxHeight: "100%",
      }}
    >
      <FormControl
        sx={{ m: 1, width: "55ch", maxWidth: "100%", maxHeight: "100%" }}
        variant="outlined"
      >
        <InputLabel htmlFor="outlined-adornment-password">Username</InputLabel>
        <OutlinedInput
          id="outlined-adornment-username"
          value={username}
          onChange={handleUsernameChange}
          label="Username"
        />
      </FormControl>
      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <InputLabel htmlFor="outlined-adornment-password">Password</InputLabel>
        <OutlinedInput
          id="outlined-adornment-password"
          type={showPassword ? "text" : "password"}
          value={password}
          onChange={handlePasswordChange}
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
      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <InputLabel htmlFor="outlined-adornment-password">URL</InputLabel>
        <OutlinedInput
          id="outlined-adornment-url"
          value={url}
          onChange={handleUrlChange}
          label="URL"
        />
      </FormControl>
      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <IntegrationNotistack />
      </FormControl>
    </Box>
  );
}



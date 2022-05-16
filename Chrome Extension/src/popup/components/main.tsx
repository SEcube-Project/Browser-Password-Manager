import * as React from "react";
import Box from "@mui/material/Box";
import BottomNavigation from "@mui/material/BottomNavigation";
import BottomNavigationAction from "@mui/material/BottomNavigationAction";
import AddCircleIcon from "@mui/icons-material/AddCircle";
import SyncLockIcon from "@mui/icons-material/SyncLock";
import FolderIcon from "@mui/icons-material/Folder";
import TabIcon from "@mui/icons-material/Tab";
import LockOpenIcon from "@mui/icons-material/LockOpen";
import LockIcon from "@mui/icons-material/Lock";
import Paper from "@mui/material/Paper";
import { fetchApi, PasswordElement } from "../../utils/api";
import { useEffect, useState } from "react";
import {
  FormControl,
  InputLabel,
  OutlinedInput,
  InputAdornment,
  IconButton,
  TextField,
  Typography,
  Button,
  Stack,
} from "@mui/material";

import CustomizedList from "./current-tab";
import AddPasswordElement from "./add-password";
import GeneratePasswordElement from "./generate-password";
import VisibilityOff from "@mui/icons-material/VisibilityOff";
import Visibility from "@mui/icons-material/Visibility";
import { SnackbarProvider, VariantType, useSnackbar } from "notistack";

const DEBUG = true;

export default function FixedBottomNavigation() {
  const [state, setState] = useState(0);
  const [passwordData, setPasswordData] = useState<PasswordElement[]>(null);
  const [showPassword, setShowPassword] = useState(false);
  const [password, setPassword] = useState("");

  useEffect(() => {
    fetchApi(DEBUG)
      .then((res) => {
        setPasswordData(res.passwords);
      })
      .catch((err) => {
        console.log(err);
      });
  }, []);

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

  function handleOnClickLogin(event: Event, newPassword: string) {
    if (DEBUG) {
      if (newPassword == "login") {
        setState(0);
        return true;
      }
    }
    // TODO: make a call to gabriel's API to check if the password is correct
  }

  function handleOnClickLock(event: Event) {
    setState(4);
  }

  function MyApp() {
    const { enqueueSnackbar } = useSnackbar();

    function handleClickVariant(newPassword: string) {
      // variant could be success, error, warning, info, or default
      if (DEBUG) {
        if (newPassword == "login") {
          setState(0);
          return true;
        } else {
          enqueueSnackbar("Wrong Password", { variant: "error" });
        }
      }
    }

    return (
      <React.Fragment>
        <Button variant="contained" onClick={() => handleClickVariant(password)} endIcon={<LockOpenIcon />}>Login</Button>
      </React.Fragment>
    );
  }

  function IntegrationNotistack() {
    return (
      <SnackbarProvider maxSnack={1} autoHideDuration={3000}>
        <MyApp />
      </SnackbarProvider>
    );
  }

  return (
    <Box sx={{ width: 400, height: 500 }}>
      {console.log("PasswordData", passwordData)}
      {state === 0 && <CustomizedList password={passwordData}/>}
      {state === 1 && <CustomizedList password={passwordData}/>}
      {state === 2 && <GeneratePasswordElement />}
      {state === 3 && <AddPasswordElement />}
      {[0, 1, 2, 3].includes(state) && (
        <Paper
          sx={{ position: "fixed", bottom: 0, left: 0, right: 0 }}
          elevation={3}
        >
          <Button
            onClick={() => handleOnClickLock(event)}
            variant="contained"
            style={{ margin: "10px", width: "95%", height: "95%" }}
            startIcon={<LockIcon />}
          >
            Lock
          </Button>

          <BottomNavigation
            showLabels
            value={state}
            onChange={(event, newState) => {
              setState(newState);
            }}
          >
            <BottomNavigationAction label="Tab" icon={<TabIcon />} />
            <BottomNavigationAction label="My Vault" icon={<FolderIcon />} />
            <BottomNavigationAction label="Generate" icon={<SyncLockIcon />} />
            <BottomNavigationAction label="Add" icon={<AddCircleIcon />} />
          </BottomNavigation>
        </Paper>
      )}
      {state === 4 && (
        <div>
          <FormControl sx={{ m: 1, width: "50ch" }} variant="outlined">
            <InputLabel htmlFor="outlined-adornment-password">
              Master Password
            </InputLabel>
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
              label="Master Password"
            />
          </FormControl>
          <FormControl sx={{ m: 1, width: "50ch" }} variant="outlined">
            <IntegrationNotistack />
          </FormControl>
        </div>
      )}
    </Box>
  );
}

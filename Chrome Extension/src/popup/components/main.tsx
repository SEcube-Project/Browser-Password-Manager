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
import {
  getAllPasswords,
  PasswordElement,
  getAllPasswordsByHostname,
} from "../../utils/api";
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
import MyVault from "./my-vault";
import AddPasswordElement from "./add-password";
import GeneratePasswordElement from "./generate-password";
import VisibilityOff from "@mui/icons-material/VisibilityOff";
import Visibility from "@mui/icons-material/Visibility";
import { SnackbarProvider, VariantType, useSnackbar } from "notistack";

export default function FixedBottomNavigation(props) {
  const [state, setState] = useState(0);
  const [allPasswords, setAllPasswords] = useState<PasswordElement[]>(null);
  const [currentTabPasswords, setCurrentTabPasswords] =
    useState<PasswordElement[]>(null);
  const [showPassword, setShowPassword] = useState(false);
  const [password, setPassword] = useState("");
  const [pageHostname, setPageHostname] = useState("");

  // receive the message from background.ts and save the value to the pageHostname

  useEffect(() => {
    if (state === 1) {
      getAllPasswords()
        .then((res) => {
          setAllPasswords(res.passwords);
        })
        .catch((err) => {
          console.log(err);
        });
    } else if (state === 0) {
      getAllPasswordsByHostname(props.hostname)
        .then((res) => {
          setCurrentTabPasswords(res.passwords);
        })
        .catch((err) => {
          console.log(err);
        });
    }
  }, [state, props.hostname]);

  useEffect(() => {
    if (props.hostname) {
      setPageHostname(props.hostname);
    }
  }
  , [props.hostname]);


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

  function handleOnClickLock(event: Event) {
    setState(4);
  }

  function MyApp() {
    const { enqueueSnackbar } = useSnackbar();

    function handleClickVariant(newPassword: string) {
      // variant could be success, error, warning, info, or default
      if (newPassword == "login") {
        setState(0);
        return true;
      } else {
        enqueueSnackbar("Wrong Password", { variant: "error" });
      }
    }

    return (
      <React.Fragment>
        <Button
          variant="contained"
          onClick={() => handleClickVariant(password)}
          endIcon={<LockOpenIcon />}
        >
          Login
        </Button>
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
      {state === 0 && <CustomizedList password={currentTabPasswords} />}
      {state === 1 && <MyVault password={allPasswords} />}
      {state === 2 && <GeneratePasswordElement />}
      {state === 3 && <AddPasswordElement url={pageHostname} />}
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

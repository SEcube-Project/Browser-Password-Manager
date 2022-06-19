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
  login,
  logout,
  getTime,
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
import { getStoredOptions, setStoredOptions } from "../../utils/storage";

export default function FixedBottomNavigation(props) {
  const [state, setState] = useState("");
  const [allPasswords, setAllPasswords] = useState<PasswordElement[]>(null);
  const [currentTabPasswords, setCurrentTabPasswords] =
    useState<PasswordElement[]>(null);
  const [showPassword, setShowPassword] = useState(false);
  const [password, setPassword] = useState("");
  const [pageHostname, setPageHostname] = useState("");
  const [isLogin, setIsLogin] = useState<boolean>(false);
  // check the local storage and then change the state if the is_locked is true

  useEffect(() => {
    const interval = setInterval(() => {
      getStoredOptions().then((options) => {
        console.log("main:", isLogin);
        if (options.is_locked && !isLogin) {
          setState("lock");
        }
      });
    }, 1000);
    return () => clearInterval(interval);
  }, [isLogin]);

  useEffect(() => {
    if (state === "myvault") {
      getAllPasswords()
        .then((res) => {
          setAllPasswords(res.passwords);
        })
        .catch((err) => {
          console.log(err);
        });
    } else if (state === "tab") {
      getAllPasswordsByHostname(pageHostname)
        .then((res) => {
          setCurrentTabPasswords(res.passwords);
        })
        .catch((err) => {
          console.log(err);
        });
    }
  }, [state, pageHostname]);

  useEffect(() => {
    if (props.hostname) {
      setPageHostname(props.hostname);
    }
  }, [props.hostname]);

  useEffect(() => {
    console.log(props.is_login);
    if (props.is_login) {
      setIsLogin(true);
    } else {
      setIsLogin(false);
    }
  }, [props.is_login]);

  useEffect(() => {
    if (props.default_state) {
      // console.log("props.default_state", props.default_state);
      setState(props.default_state);
    }
  }, [props.default_state]);

  const handleClickShowPassword = () => {
    setShowPassword(!showPassword);
  };

  const handleMouseDownPassword = (
    event: React.MouseEvent<HTMLButtonElement>
  ) => {
    event.preventDefault();
  };

  const handlePasswordChange = (event) => {
    setIsLogin(true);
    setPassword(event.target.value);
  };

  function handleOnClickLock(event: Event) {
    setIsLogin(true);
    setState("lock");
    logout();
    getStoredOptions().then((options) => {
      setStoredOptions({ ...options, is_locked: true });
    });
  }

  function MyApp() {
    const { enqueueSnackbar } = useSnackbar();

    function handleClickVariant(newPassword: string) {
      // console.log("newPassword", newPassword);
      getTime().then((res) => {
        getStoredOptions().then((options) => {
          login(newPassword, res.time + options.lock_after_minutes * 60)
            .then((res) => {
              if (res === true) {
                setIsLogin(false);
                enqueueSnackbar("Login Successful", {
                  variant: "success",
                });
                getAllPasswordsByHostname(pageHostname)
                  .then((res) => {
                    if (res) {
                      setCurrentTabPasswords(res.passwords);
                      getStoredOptions().then((options) => {
                        setStoredOptions({
                          ...options,
                          is_locked: false,
                        });
                      });
                    }
                  })
                  .catch((err) => {
                    console.log(err);
                  });
                setState("tab");
              } else {
                enqueueSnackbar("Login Failed", {
                  variant: "error",
                });
              }
            })
            .catch((err) => {
              console.log(err);
            });
        });
      });
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
      {state === "tab" && <CustomizedList password={currentTabPasswords} />}
      {state === "myvault" && <MyVault password={allPasswords} />}
      {state === "generate" && <GeneratePasswordElement />}
      {state === "add" && <AddPasswordElement url={pageHostname} />}
      {["tab", "myvault", "generate", "add"].includes(state) && (
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
            <BottomNavigationAction
              label="Tab"
              value="tab"
              icon={<TabIcon />}
            />
            <BottomNavigationAction
              label="My Vault"
              value="myvault"
              icon={<FolderIcon />}
            />
            <BottomNavigationAction
              label="Generate"
              value="generate"
              icon={<SyncLockIcon />}
            />
            <BottomNavigationAction
              label="Add"
              value="add"
              icon={<AddCircleIcon />}
            />
          </BottomNavigation>
        </Paper>
      )}
      {state === "lock" && (
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
          <div>
            {" "}
            {" "}
            {" "}
            {" "}
            {" "}
            {" "}
            {" "}
            <p style={{color: "grey", position: "fixed", bottom: "0px", left: "10px", right: "10px", textAlign: "center"}}>
              Tesing version: if you are not able to login go to{" "}
              <a href="https://127.0.0.1:5000/" target="_blank">
                https://127.0.0.1:5000/
              </a>{" "}
              and accept the certificate
            </p>
          </div>
        </div>
      )}
    </Box>
  );
}

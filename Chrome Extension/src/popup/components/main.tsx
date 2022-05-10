import * as React from "react";
import Box from "@mui/material/Box";
import BottomNavigation from "@mui/material/BottomNavigation";
import BottomNavigationAction from "@mui/material/BottomNavigationAction";
import AddCircleIcon from "@mui/icons-material/AddCircle";
import SyncLockIcon from "@mui/icons-material/SyncLock";
import FolderIcon from "@mui/icons-material/Folder";
import TabIcon from "@mui/icons-material/Tab";
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
} from "@mui/material";

import ListPasswordElements from "./current-tab";
import AddPasswordElement from "./add-password";
import GeneratePasswordElement from "./generate-password";

const DEBUG = true;

export default function FixedBottomNavigation() {
  const [state, setState] = useState(0);
  const [passwordData, setPasswordData] = useState<PasswordElement[]>(null);

  useEffect(() => {
    fetchApi(DEBUG)
      .then((res) => {
        console.log(res);
        setPasswordData(res);
      })
      .catch((err) => {
        console.log(err);
      });
  }, []);

  return (
    <Box sx={{ width: 400, height: 400 }}>
      {state === 0 && <ListPasswordElements passwordData={passwordData} />}
      {state === 1 && <ListPasswordElements passwordData={passwordData} />}
      {state === 2 && <GeneratePasswordElement />}
      {state === 3 && <AddPasswordElement />}
      <Paper
        sx={{ position: "fixed", bottom: 0, left: 0, right: 0 }}
        elevation={3}
      >
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
    </Box>
  );
}

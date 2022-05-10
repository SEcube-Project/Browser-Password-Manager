import * as React from "react";
import Box from "@mui/material/Box";
import CssBaseline from "@mui/material/CssBaseline";
import BottomNavigation from "@mui/material/BottomNavigation";
import BottomNavigationAction from "@mui/material/BottomNavigationAction";
import LockIcon from "@mui/icons-material/Lock";
import AddCircleIcon from "@mui/icons-material/AddCircle";
import SyncLockIcon from "@mui/icons-material/SyncLock";
import FolderIcon from "@mui/icons-material/Folder";
import TabIcon from "@mui/icons-material/Tab";
import Paper from "@mui/material/Paper";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemAvatar from "@mui/material/ListItemAvatar";
import ListItemText from "@mui/material/ListItemText";
import Avatar from "@mui/material/Avatar";
import { fetchApi, PasswordElement } from "../../utils/api";
import { useEffect, useState } from "react";

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
    <Box sx={{ pb: 7, width: 500, height: 500 }}>
      <CssBaseline />
      <ListPasswordElements passwordData={passwordData} state={state} />
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

function ListPasswordElements(props) {
  if (props.state === 0) {
    return (
      <List>
        {props.passwordData?.map(({ username, password, url }, index) => (
          <ListItem button key={index + url}>
            <ListItemText primary={username} />
            <ListItemText primary={password} />
            <ListItemText primary={url} />
          </ListItem>
        ))}
      </List>
    );
  }
  return(null);
}

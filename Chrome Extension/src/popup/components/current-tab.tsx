import * as React from "react";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemText from "@mui/material/ListItemText";
import ListSubheader from "@mui/material/ListSubheader";
import AccountCircle from "@mui/icons-material/AccountCircle";
import Password from "@mui/icons-material/Password";
import Link from "@mui/icons-material/Link";
import Box from "@mui/material/Box";
import { styled, ThemeProvider, createTheme } from "@mui/material/styles";
import Divider from "@mui/material/Divider";
import ListItemButton from "@mui/material/ListItemButton";
import ListItemIcon from "@mui/material/ListItemIcon";
import Paper from "@mui/material/Paper";
import IconButton from "@mui/material/IconButton";
import Tooltip from "@mui/material/Tooltip";
import ArrowRight from "@mui/icons-material/ArrowRight";
import KeyboardArrowDown from "@mui/icons-material/KeyboardArrowDown";
import Home from "@mui/icons-material/Home";
import Settings from "@mui/icons-material/Settings";
import People from "@mui/icons-material/People";
import PermMedia from "@mui/icons-material/PermMedia";
import Dns from "@mui/icons-material/Dns";
import Public from "@mui/icons-material/Public";

const data = [
  { icon: <AccountCircle />, label: "Username" },
  { icon: <Password />, label: "Password" },
  { icon: <Link />, label: "URL" },
];

const FireNav = styled(List)<{ component?: React.ElementType }>({
  "& .MuiListItemButton-root": {
    paddingLeft: 24,
    paddingRight: 24,
  },
  "& .MuiListItemIcon-root": {
    minWidth: 0,
    marginRight: 16,
  },
  "& .MuiSvgIcon-root": {
    fontSize: 20,
  },
});

export default function PinnedSubheaderList() {
  const [open, setOpen] = React.useState([false]);

  const handleClick = (index: number) => {
    setOpen((prevOpen) => {
      const newOpen = [...prevOpen];
      newOpen[index] = !newOpen[index];
      return newOpen;
    });
  };

  return (
    <List
      sx={{
        width: "100%",
        maxWidth: 400,
        bgcolor: "background.paper",
        position: "relative",
        overflow: "auto",
        maxHeight: 280,
        "& ul": { padding: 0 },
      }}
      subheader={<li />}
    >
      {["A", "B", "C", "D", "E"].map((sectionId) => (
        <li key={`section-${sectionId}`}>
          <ul>
            <ListSubheader>{`${sectionId}`}</ListSubheader>

            {[0, 1, 2].map((item) => (
              <ListItem key={`item-${sectionId}-${item}`}>
                {/* <Box
                  sx={{
                    bgcolor: open ? "rgba(239, 15, 15, 0.86)" : null,
                    pb: open ? 2 : 2,
                  }}
                > */}
                  <ListItemButton
                    alignItems="flex-start"
                    onClick={() => handleClick(item)}
                    sx={{
                      px: 3,
                      pt: 2.5,
                      pb: open ? 0 : 2.5,
                      "&:hover, &:focus": {
                        "& svg": { opacity: open ? 1 : 1 },
                      },
                    }}
                  >
                    <ListItemText
                      primary="Build"
                      primaryTypographyProps={{
                        fontSize: 15,
                        fontWeight: "medium",
                        lineHeight: "20px",
                        mb: "2px",
                      }}
                      secondary="Authentication, Firestore Database, Realtime Database, Storage, Hosting, Functions, and Machine Learning"
                      secondaryTypographyProps={{
                        noWrap: true,
                        fontSize: 12,
                        lineHeight: "16px",
                        color: open
                          ? "rgba(239, 15, 15, 0.86)"
                          : "rgba(239, 15, 15, 0.86)",
                      }}
                      sx={{ my: 0 }}
                    />
                    <KeyboardArrowDown
                      sx={{
                        mr: -1,
                        opacity: 0,
                        transform: open ? "rotate(-180deg)" : "rotate(0)",
                        transition: "0.2s",
                      }}
                    />
                  </ListItemButton>
                  {open[item] &&
                    data.map((item) => (
                      <ListItemButton
                        key={item.label}
                        sx={{ py: 0, minHeight: 32, color: "rgba(0, 0, 0, 1)" }}
                      >
                        <ListItemIcon sx={{ color: "inherit" }}>
                          {item.icon}
                        </ListItemIcon>
                        <ListItemText
                          primary={item.label}
                          primaryTypographyProps={{
                            fontSize: 14,
                            fontWeight: "medium",
                          }}
                        />
                      </ListItemButton>
                    ))}
                {/* </Box> */}
              </ListItem>
            ))}
          </ul>
        </li>
      ))}
    </List>
  );
}

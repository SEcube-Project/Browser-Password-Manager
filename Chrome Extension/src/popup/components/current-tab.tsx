import * as React from "react";
import Box from "@mui/material/Box";
import { styled, ThemeProvider, createTheme } from "@mui/material/styles";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemButton from "@mui/material/ListItemButton";
import ListItemIcon from "@mui/material/ListItemIcon";
import ListItemText from "@mui/material/ListItemText";
import Paper from "@mui/material/Paper";
import KeyboardArrowDown from "@mui/icons-material/KeyboardArrowDown";
import { ListSubheader } from "@mui/material";
import AccountCircle from "@mui/icons-material/AccountCircle";
import Password from "@mui/icons-material/Password";
import Link from "@mui/icons-material/Link";
import { color } from "@mui/system";

var data = [
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

export default function CustomizedList() {
  const [open, setOpen] = React.useState([false]);

  const handleClick = (index: number) => {
    setOpen((prevOpen) => {
      const newOpen = [...prevOpen];
      newOpen[index] = !newOpen[index];
      return newOpen;
    });
  };

  return (
    <Box>
      <ThemeProvider
        theme={createTheme({
          components: {
            MuiListItemButton: {
              defaultProps: {
                disableTouchRipple: true,
              },
            },
          },
          palette: {
            primary: { main: "rgba(25, 118, 210, 0.3)" },
            background: { paper: "rgba(25, 118, 210, 0.3)" },
          },
        })}
      >
        <Paper elevation={0} sx={{ maxWidth: "100%" }}>
          <FireNav component="nav" disablePadding>
            <List
              sx={{
                width: "100%",
                height: "380px",
                maxWidth: "100%",
                position: "relative",
                overflow: "auto",
                "& ul": { padding: 0 },
              }}
              subheader={<li />}
            >
              {["A", "B", "C", "D", "E"].map((sectionId) => (
                <li key={`section-${sectionId}`}>
                  <ul>
                    <ListSubheader sx={{ bgcolor: 
                      "rgba(10, 50, 105, 1)", color: "white"}}>{`${sectionId}`}</ListSubheader>

                    {[0, 1, 2].map((item) => (
                      <ListItem key={`item-${sectionId}-${item}`}>
                        <Box
                          sx={{
                            bgcolor: open[item]
                              ? "rgba(25, 118, 210, 0.7)"
                              : "rgba(25, 118, 210, 0.7)",
                            width: "360px",
                          }}
                        >
                          <ListItemButton
                            alignItems="flex-start"
                            onClick={() => handleClick(item)}
                            sx={{
                              px: 3,
                              pt: 2.5,
                              pb: open[item] ? 0 : 2.5,
                              "&:hover, &:focus": {
                                "& svg": { opacity: open[item] ? 1 : 1 },
                              },
                            }}
                          >
                            <ListItemText
                              primary="shdfbdshfbjhsdfbshbsdbhcbshcbsdschjdsbcjhscbjshcbjshdbcsjbcsjhdbcsjdhbhjcsjcbsjcbsjcbshcsjcsjcbjcbsjcbscbsjc"
                              primaryTypographyProps={{
                                noWrap: true,
                                fontSize: 15,
                                fontWeight: "medium",
                                lineHeight: "20px",
                                mb: "2px",
                              }}
                              secondary="Username, Password, URL, shdfbdshfbjhsdfbshbsdbhcbshcbsdschjdsbcjhscbjshcbjshdbcsjbcsjhdbcsjdhbhjcsjcbsjcbsjcbshcsjcsjcbjcbsjcbscbsjc"
                              secondaryTypographyProps={{
                                noWrap: true,
                                fontSize: 12,
                                lineHeight: "16px",
                                color: open[item]
                                  ? "rgba(0, 0, 0, 1)"
                                  : "rgba(0, 0, 0, 1)",
                              }}
                              sx={{ my: 0 }}
                            />
                            <KeyboardArrowDown
                              sx={{
                                mr: -1,
                                opacity: 0,
                                transform: open[item]
                                  ? "rotate(-180deg)"
                                  : "rotate(0)",
                                transition: "0.2s",
                              }}
                            />
                          </ListItemButton>
                          {open[item] &&
                            data.map((item) => (
                              <ListItemButton
                                key={item.label}
                                sx={{
                                  py: 0,
                                  minHeight: 32,
                                  color: "rgba(0, 0, 0, 1)",
                                }}
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
                        </Box>
                      </ListItem>
                    ))}
                  </ul>
                </li>
              ))}
            </List>
          </FireNav>
        </Paper>
      </ThemeProvider>
    </Box>
  );
}
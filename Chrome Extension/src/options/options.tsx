import React from 'react'
import ReactDOM from 'react-dom'
import { fetchApi } from '../utils/api'
import './options.css'

const App: React.FC<{}> = () => {
  return (
    <div>
      <img src="password.png" />
    </div>
  )
}

const root = document.createElement('div')
document.body.appendChild(root)
ReactDOM.render(<App />, root)

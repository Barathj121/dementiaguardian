import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import Alert from './components/alert';

function App() {
  const [count, setCount] = useState(0)

  return (
    <div>
      <Alert/>
    </div>
  )
}

export default App

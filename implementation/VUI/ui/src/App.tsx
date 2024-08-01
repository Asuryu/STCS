import './App.css'
import { useEffect } from 'react'

function App() {
  const handleSocket = () => {
    const ws = new WebSocket('ws://server:8084')
    ws.onopen = () => {
      console.log('connected')
    }
    ws.onmessage = (msg) => {
      console.log(msg.data)
    }
    ws.onclose = () => {
      console.log('disconnected')
    }
  }

  useEffect(() => {
    handleSocket();
  }, [])

  return (
    <>
      <h1>oi12</h1>
    </>
  )
}

export default App;

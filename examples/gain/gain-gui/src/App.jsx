import {useState} from 'react'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    const gainParam = window.params["Gain"];
    return (
        <>
            <div>
                <p className="label">gain</p>
                <SliderComponent pid={gainParam.id}
                                 min={gainParam.min}
                                 max={gainParam.max}
                                 initial={gainParam.initial}
                />
            </div>
        </>
    )
}

export default App

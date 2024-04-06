import Content from "./Content";
import Footer from "./Footer";
import NavBar from "./NavBar";

export default function Main(){
    return(
        <div className="main-panel">
            <NavBar/>
            <Content/>
            <Footer/>
        </div>
    )
}
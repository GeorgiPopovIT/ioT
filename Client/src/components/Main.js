import Content from "./Content";
import Footer from "./Footer";
import NavBar from "./NavBar";

const Main = () => {
    return(
        <div className="main-panel">
            <NavBar/>
            <Content/>
            <Footer/>
        </div>
    )
}

export default Main;
package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"log"
	"net/http"
	"time"

	"github.com/roman-mazur/design-practice-3-template/httptools"
	"github.com/roman-mazur/design-practice-3-template/signal"
)

const UintSize = 32 << (^uint(0) >> 32 & 1) // 32 or 64

const MaxInt  = 1<<(UintSize-1) - 1

var (
	port = flag.Int("port", 8090, "load balancer port")
	timeoutSec = flag.Int("timeout-sec", 3, "request timeout time in seconds")
	https = flag.Bool("https", false, "whether backends support HTTPs")

	traceEnabled = flag.Bool("trace", false, "whether to include tracing information into responses")
)

var (
	timeout = time.Duration(*timeoutSec) * time.Second
	serversPool = []string{
		"server1:8080",
		"server2:8080",
		"server3:8080",
	}
	serverCount = make(map[string]Server, len(serversPool))
)

type Server struct {
	connectionCount int
	isHealthy bool
}

func min(m map[string] Server) string {
	min := MaxInt
	k := ""
	for s, c := range m {
		if min >= c.connectionCount && c.isHealthy {
			k = s
			min = c.connectionCount
		}
	}
	return k
}

func scheme() string {
	if *https {
		return "https"
	}
	return "http"
}

func health(dst string) bool {
	ctx, _ := context.WithTimeout(context.Background(), timeout)
	req, _ := http.NewRequestWithContext(ctx, "GET",
		fmt.Sprintf("%s://%s/health", scheme(), dst), nil)
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return false
	}
	if resp.StatusCode != http.StatusOK {
		return false
	}
	return true
}

func forward(dst string, rw http.ResponseWriter, r *http.Request) error {
	s1 := serverCount[dst]
	s1.connectionCount++
	serverCount[dst] = s1
	ctx, _ := context.WithTimeout(r.Context(), timeout)
	fwdRequest := r.Clone(ctx)
	fwdRequest.RequestURI = ""
	fwdRequest.URL.Host = dst
	fwdRequest.URL.Scheme = scheme()
	fwdRequest.Host = dst

	resp, err := http.DefaultClient.Do(fwdRequest)
	if err == nil {
		for k, values := range resp.Header {
			for _, value := range values {
				rw.Header().Add(k, value)
			}
		}
		if *traceEnabled {
			rw.Header().Set("lb-from", dst)
		}
		log.Println("fwd", resp.StatusCode, resp.Request.URL)
		rw.WriteHeader(resp.StatusCode)
		defer resp.Body.Close()
		_, err := io.Copy(rw, resp.Body)
		if err != nil {
			log.Printf("Failed to write response: %s", err)
		}
		s2 := serverCount[dst]
		s2.connectionCount--
		serverCount[dst] = s2
		return nil
	} else {
		log.Printf("Failed to get response from %s: %s", dst, err)
		rw.WriteHeader(http.StatusServiceUnavailable)
		s2 := serverCount[dst]
		s2.connectionCount--
		serverCount[dst] = s2
		return err
	}
}

func main() {
	flag.Parse()
	// TODO: Використовуйте дані про стан сервреа, щоб підтримувати список тих серверів, яким можна відправляти ззапит.
	for _, server := range serversPool {
		server := server
		serverCount[server] = Server{0, true}
		go func() {
			for range time.Tick(10 * time.Second) {
				log.Println(server, health(server))
			}
		}()
		go func() {
			for range time.Tick(1 * time.Second) {
				s1 := serverCount[server]
				s1.isHealthy = health(server)
				serverCount[server] = s1
			}
		}()
	}

	frontend := httptools.CreateServer(*port, http.HandlerFunc(func(rw http.ResponseWriter, r *http.Request) {
		// TODO: Рееалізуйте свій алгоритм балансувальника.
		forward(min(serverCount), rw, r)
	}))

	log.Println("Starting load balancer...")
	log.Printf("Tracing support enabled: %t", *traceEnabled)
	frontend.Start()
	signal.WaitForTerminationSignal()
}
